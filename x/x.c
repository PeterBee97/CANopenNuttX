#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <limits.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <nuttx/can.h>
#include <netpacket/can.h>
#include <signal.h>

/******************************************************************************/
int main (int argc, char *argv[]) {
	int nbytes;
    fd_set rdfs;
	int s[MAXSOCK];
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	if ((s[i] = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("CAN socket creation failed");
        exit(EXIT_FAILURE);
	}

	strcpy(ifr.ifr_name, "can0" );
	if (ioctl(s[i], SIOCGIFINDEX, &ifr) < 0) {
        perror("SIOCGIFINDEX");
        exit(EXIT_FAILURE);
    }

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s[i], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Binding failed");
		exit(EXIT_FAILURE);
	}

	struct can_filter rfilter[1];

	rfilter[0].can_id   = 0x105;
	rfilter[0].can_mask = 0xFF0;
	//rfilter[1].can_id   = 0x200;
	//rfilter[1].can_mask = 0x700;

	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

	/* these settings are static and can be held out of the hot path */
	iov.iov_base = &frame;
	msg.msg_name = &addr;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = &ctrlmsg;

	while (running) {

		FD_ZERO(&rdfs);
		for (i=0; i<currmax; i++)
			FD_SET(s[i], &rdfs);

		if (timeout_current)
			*timeout_current = timeout_config;

		if ((ret = select(s[currmax-1]+1, &rdfs, NULL, NULL, timeout_current)) <= 0) {
			//perror("select");
			running = 0;
			continue;
		}

		for (i=0; i<currmax; i++) {  /* check all CAN RAW sockets */

			if (FD_ISSET(s[i], &rdfs)) {

				int idx;

				/* these settings may be modified by recvmsg() */
				iov.iov_len = sizeof(frame);
				msg.msg_namelen = sizeof(addr);
				msg.msg_controllen = sizeof(ctrlmsg);
				msg.msg_flags = 0;

				nbytes = recvmsg(s[i], &msg, 0);
				idx = idx2dindex(addr.can_ifindex, s[i]);

				if (nbytes < 0) {
					if ((errno == ENETDOWN) && !down_causes_exit) {
						fprintf(stderr, "%s: interface down\n", devname[idx]);
						continue;
					}
					perror("read");
					return 1;
				}

				if ((size_t)nbytes == CAN_MTU)
					maxdlen = CAN_MAX_DLEN;
				else if ((size_t)nbytes == CANFD_MTU)
					maxdlen = CANFD_MAX_DLEN;
				else {
					fprintf(stderr, "read: incomplete CAN frame\n");
					return 1;
				}

				if (count && (--count == 0))
					running = 0;

				for (cmsg = CMSG_FIRSTHDR(&msg);
				     cmsg && (cmsg->cmsg_level == SOL_SOCKET);
				     cmsg = CMSG_NXTHDR(&msg,cmsg)) {
					if (cmsg->cmsg_type == SO_TIMESTAMP) {
						memcpy(&tv, CMSG_DATA(cmsg), sizeof(tv));
					} else if (cmsg->cmsg_type == SO_TIMESTAMPING) {

						struct timespec *stamp = (struct timespec *)CMSG_DATA(cmsg);

						/*
						 * stamp[0] is the software timestamp
						 * stamp[1] is deprecated
						 * stamp[2] is the raw hardware timestamp
						 * See chapter 2.1.2 Receive timestamps in
						 * linux/Documentation/networking/timestamping.txt
						 */
						tv.tv_sec = stamp[2].tv_sec;
						tv.tv_usec = stamp[2].tv_nsec/1000;
					} else if (cmsg->cmsg_type == SO_RXQ_OVFL)
						memcpy(&dropcnt[i], CMSG_DATA(cmsg), sizeof(__u32));
				}

				/* check for (unlikely) dropped frames on this specific socket */
				if (dropcnt[i] != last_dropcnt[i]) {

					__u32 frames = dropcnt[i] - last_dropcnt[i];

					if (silent != SILENT_ON)
						printf("DROPCOUNT: dropped %" PRId32 " CAN frame%s on '%s' socket (total drops %" PRId32 ")\n",
						       (uint32_t)frames, (frames > 1)?"s":"", devname[idx], (uint32_t)dropcnt[i]);

					if (log)
						fprintf(logfile, "DROPCOUNT: dropped %" PRId32 " CAN frame%s on '%s' socket (total drops %" PRId32 ")\n",
							(uint32_t)frames, (frames > 1)?"s":"", devname[idx], (uint32_t)dropcnt[i]);

					last_dropcnt[i] = dropcnt[i];
				}

				/* once we detected a EFF frame indent SFF frames accordingly */
				if (frame.can_id & CAN_EFF_FLAG)
					view |= CANLIB_VIEW_INDENT_SFF;

				if (log) {
					char buf[CL_CFSZ]; /* max length */

					/* log CAN frame with absolute timestamp & device */
					sprint_canframe(buf, &frame, 0, maxdlen);
					fprintf(logfile, "(%010ju.%06ld) %*s %s\n",
						(uintmax_t)tv.tv_sec, tv.tv_usec,
						max_devname_len, devname[idx], buf);
				}

				if ((logfrmt) && (silent == SILENT_OFF)){
					char buf[CL_CFSZ]; /* max length */

					/* print CAN frame in log file style to stdout */
					sprint_canframe(buf, &frame, 0, maxdlen);
					printf("(%010ju.%06ld) %*s %s\n",
					       (uintmax_t)tv.tv_sec, tv.tv_usec,
					       max_devname_len, devname[idx], buf);
					goto out_fflush; /* no other output to stdout */
				}

				if (silent != SILENT_OFF){
					if (silent == SILENT_ANI) {
						printf("%c\b", anichar[silentani%=MAXANI]);
						silentani++;
					}
					goto out_fflush; /* no other output to stdout */
				}

				printf(" %s", (color>2)?col_on[idx%MAXCOL]:"");

				switch (timestamp) {

				case 'a': /* absolute with timestamp */
					printf("(%010ju.%06ld) ",
						   (uintmax_t)tv.tv_sec, tv.tv_usec);
					break;

				case 'A': /* absolute with date */
				{
					struct tm tm;
					char timestring[25];

					tm = *localtime(&tv.tv_sec);
					strftime(timestring, 24, "%Y-%m-%d %H:%M:%S", &tm);
					printf("(%s.%06ld) ", timestring, tv.tv_usec);
				}
				break;

				case 'd': /* delta */
				case 'z': /* starting with zero */
				{
					struct timeval diff;

					if (last_tv.tv_sec == 0)   /* first init */
						last_tv = tv;
					diff.tv_sec  = tv.tv_sec  - last_tv.tv_sec;
					diff.tv_usec = tv.tv_usec - last_tv.tv_usec;
					if (diff.tv_usec < 0)
						diff.tv_sec--, diff.tv_usec += 1000000;
					if (diff.tv_sec < 0)
						diff.tv_sec = diff.tv_usec = 0;
					printf("(%03ju.%06ld) ",
						   (uintmax_t)diff.tv_sec, diff.tv_usec);

					if (timestamp == 'd')
						last_tv = tv; /* update for delta calculation */
				}
				break;

				default: /* no timestamp output */
					break;
				}

				printf(" %s", (color && (color<3))?col_on[idx%MAXCOL]:"");
				printf("%*s", max_devname_len, devname[idx]);

				if (extra_msg_info) {

					if (msg.msg_flags & MSG_DONTROUTE)
						printf ("  TX %s", extra_m_info[frame.flags & 3]);
					else
						printf ("  RX %s", extra_m_info[frame.flags & 3]);
				}

				printf("%s  ", (color==1)?col_off:"");

				fprint_long_canframe(stdout, &frame, NULL, view, maxdlen);

				printf("%s", (color>1)?col_off:"");
				printf("\n");
			}

		out_fflush:
			fflush(stdout);
		}
	}

	for (i=0; i<currmax; i++)
		close(s[i]);

	if (nbytes < 0) {
		perror("Read");
		return 1;
	}

	printf("0x%03X [%d] ",frame.can_id, frame.can_dlc);

	for (i = 0; i < frame.can_dlc; i++)
		printf("%02X ",frame.data[i]);

	printf("\r\n");

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

    return 0;
}
