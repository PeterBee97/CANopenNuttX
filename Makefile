############################################################################
# apps/canutils/canopennode/Makefile
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
###########################################################################

# Standard includes

include $(APPDIR)/Make.defs

CSRCS += CO_driver.c
CSRCS += CO_error.c
CSRCS += CO_epoll_interface.c
CSRCS += CO_storageLinux.c
CSRCS += CANopenNode/301/CO_ODinterface.c
CSRCS += CANopenNode/301/CO_NMT_Heartbeat.c
CSRCS += CANopenNode/301/CO_HBconsumer.c
CSRCS += CANopenNode/301/CO_Emergency.c
CSRCS += CANopenNode/301/CO_SDOserver.c
CSRCS += CANopenNode/301/CO_SDOclient.c
CSRCS += CANopenNode/301/CO_TIME.c
CSRCS += CANopenNode/301/CO_SYNC.c
CSRCS += CANopenNode/301/CO_PDO.c
CSRCS += CANopenNode/301/crc16-ccitt.c
CSRCS += CANopenNode/301/CO_fifo.c
CSRCS += CANopenNode/303/CO_LEDs.c
CSRCS += CANopenNode/304/CO_GFC.c
CSRCS += CANopenNode/304/CO_SRDO.c
CSRCS += CANopenNode/305/CO_LSSslave.c
CSRCS += CANopenNode/305/CO_LSSmaster.c
CSRCS += CANopenNode/309/CO_gateway_ascii.c
CSRCS += CANopenNode/storage/CO_storage.c
CSRCS += CANopenNode/extra/CO_trace.c
CSRCS += CANopenNode/CANopen.c
CSRCS += CANopenNode/example/OD.c

CFLAGS += ${INCDIR_PREFIX}. ${INCDIR_PREFIX}CANopenNode ${INCDIR_PREFIX}CANopenNode/example
CFLAGS += -Wno-shadow -Wno-undef
CFLAGS += -DCO_CONFIG_STORAGE=0 -DCO_CONFIG_GTW=0x1FF
CFLAGS += -DCO_CONFIG_GTWA_COMM_BUF_SIZE=100 -DCO_CONFIG_GTWA_LOG_BUF_SIZE=1000 -DCO_CONFIG_GTW_BLOCK_DL_LOOP=3
CFLAGS += -DCO_CONFIG_GTW_NET_MIN=0 -DCO_CONFIG_GTW_NET_MAX=0xFFFF

MODULE = $(CONFIG_CANUTILS_CANOPENNODE)

# CANopenNode tools

ifeq ($(CONFIG_CANUTILS_CANOPENNODE_TOOLS_CANOPEND),y)
PROGNAME   = canopend
PRIORITY   = $(CONFIG_CANUTILS_CANOPENNODE_TOOLS_CANOPEND_PRIORITY)
STACKSIZE  = $(CONFIG_CANUTILS_CANOPENNODE_TOOLS_CANOPEND_STACKSIZE)
MAINSRC    = CO_main_basic.c
endif

ifeq ($(CONFIG_CANUTILS_CANOPENNODE_TOOLS_COCOMM),y)
PROGNAME  += cocomm
PRIORITY  += $(CONFIG_CANUTILS_CANOPENNODE_TOOLS_COCOMM_PRIORITY)
STACKSIZE += $(CONFIG_CANUTILS_CANOPENNODE_TOOLS_COCOMM_STACKSIZE)
MAINSRC   += cocomm/cocomm.c
endif

ifeq ($(CONFIG_CANUTILS_CANOPENNODE_TOOLS_X),y)
PROGNAME  += x
PRIORITY  += $(CONFIG_CANUTILS_CANOPENNODE_TOOLS_X_PRIORITY)
STACKSIZE += $(CONFIG_CANUTILS_CANOPENNODE_TOOLS_X_STACKSIZE)
MAINSRC   += x/x.c
endif

include $(APPDIR)/Application.mk
