# ##############################################################################
# apps/canutils/canopennode/CMakeLists.txt
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

# if(CONFIG_CANUTILS_CANOPENNODE)

#   # ############################################################################
#   # Config and Fetch lelycanopen lib
#   # ############################################################################

#   set(CANOPENNODE_DIR ${CMAKE_CURRENT_LIST_DIR}/canopennode)

#   if(NOT EXISTS ${CANOPENNODE_DIR})
#     FetchContent_Declare(
#       lelycanopen_fetch
#       URL ${CONFIG_CANUTILS_CANOPENNODE_URL}/lely-core-${CONFIG_CANUTILS_CANOPENNODE_VERSION}.tar.gz
#           SOURCE_DIR
#           ${CMAKE_CURRENT_LIST_DIR}/canopennode
#           BINARY_DIR
#           ${CMAKE_BINARY_DIR}/apps/canutils/canopennode/canopennode
#       PATCH_COMMAND patch -p1 -d ${CMAKE_CURRENT_LIST_DIR}/canopennode <
#                     ${CMAKE_CURRENT_LIST_DIR}/0001-NuttX-port.patch
#       DOWNLOAD_NO_PROGRESS true
#       TIMEOUT 30)

#     FetchContent_GetProperties(lelycanopen_fetch)

#     if(NOT lelycanopen_fetch_POPULATED)
#       FetchContent_Populate(lelycanopen_fetch)
#     endif()
#   endif()

#   nuttx_add_library(lelycanopen STATIC)

#   # ############################################################################
#   # Flags
#   # ############################################################################

#   set(CFLAGS -Wno-shadow -Wno-undef -DHAVE_CONFIG_H=1)

#   # ############################################################################
#   # Sources
#   # ############################################################################

#   set(CSRCS)

#   # CAN network object
#   list(APPEND CSRCS ${CANOPENNODE_DIR}/src/can/buf.c
#        ${CANOPENNODE_DIR}/src/can/msg.c ${CANOPENNODE_DIR}/src/can/vci.c
#        ${CANOPENNODE_DIR}/src/can/net.c)
#   # CANopen library
#   list(
#     APPEND
#     CSRCS
#     ${CANOPENNODE_DIR}/src/co/crc.c
#     ${CANOPENNODE_DIR}/src/co/dev.c
#     ${CANOPENNODE_DIR}/src/co/nmt.c
#     ${CANOPENNODE_DIR}/src/co/nmt_hb.c
#     ${CANOPENNODE_DIR}/src/co/nmt_srv.c
#     ${CANOPENNODE_DIR}/src/co/obj.c
#     ${CANOPENNODE_DIR}/src/co/pdo.c
#     ${CANOPENNODE_DIR}/src/co/sdo.c
#     ${CANOPENNODE_DIR}/src/co/ssdo.c
#     ${CANOPENNODE_DIR}/src/co/type.c
#     ${CANOPENNODE_DIR}/src/co/val.c)

#   if(CONFIG_CANUTILS_CANOPENNODE_TIME)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/time.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_CSDO)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/csdo.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_DCF)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/dcf.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_EMCY)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/emcy.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_GW)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/gw.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_GW_TXT)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/gw_txt.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_LSS)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/lss.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_NMTBOOT)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/nmt_boot.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_NMTCFG)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/nmt_cfg.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_RPDO)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/rpdo.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_SDEV)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/sdev.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_SYNC)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/sync.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_TPDO)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/tpdo.c)
#   endif()

#   if(CONFIG_CANUTILS_CANOPENNODE_WTM)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/co/wtm.c)
#   endif()

#   # utils

#   if(CONFIG_CANUTILS_CANOPENNODE_DIAG)
#     list(APPEND CSRCS ${CANOPENNODE_DIR}/src/util/diag.c)
#   endif()

#   list(
#     APPEND
#     CSRCS
#     ${CANOPENNODE_DIR}/src/util/bits.c
#     ${CANOPENNODE_DIR}/src/util/bitset.c
#     ${CANOPENNODE_DIR}/src/util/cmp.c
#     ${CANOPENNODE_DIR}/src/util/config.c
#     ${CANOPENNODE_DIR}/src/util/config_ini.c
#     ${CANOPENNODE_DIR}/src/util/dllist.c
#     ${CANOPENNODE_DIR}/src/util/endian.c
#     ${CANOPENNODE_DIR}/src/util/errnum.c
#     ${CANOPENNODE_DIR}/src/util/frbuf.c
#     ${CANOPENNODE_DIR}/src/util/fwbuf.c
#     ${CANOPENNODE_DIR}/src/util/lex.c
#     ${CANOPENNODE_DIR}/src/util/membuf.c
#     ${CANOPENNODE_DIR}/src/util/pheap.c
#     ${CANOPENNODE_DIR}/src/util/print.c
#     ${CANOPENNODE_DIR}/src/util/rbtree.c
#     ${CANOPENNODE_DIR}/src/util/stop.c
#     ${CANOPENNODE_DIR}/src/util/time.c
#     ${CANOPENNODE_DIR}/src/util/ustring.c)

#   # Lely IO lib
#   if(CONFIG_CANUTILS_CANOPENNODE_IOLIB)
#     list(
#       APPEND
#       CSRCS
#       ${CANOPENNODE_DIR}/src/io/handle.c
#       ${CANOPENNODE_DIR}/src/io/io.c
#       ${CANOPENNODE_DIR}/src/io/pipe.c
#       ${CANOPENNODE_DIR}/src/io/poll.c
#       ${CANOPENNODE_DIR}/src/io2/posix/poll.c
#       ${CANOPENNODE_DIR}/src/io2/ctx.c
#       ${CANOPENNODE_DIR}/src/io/can.c
#       ${CANOPENNODE_DIR}/src/can/socket.c)
#   endif()

#   # ############################################################################
#   # Include Directory
#   # ############################################################################

#   set(INCDIR ${CANOPENNODE_DIR}/include ${NUTTX_APPS_DIR}/include/canutils/lely)

#   # ############################################################################
#   # Applications Configuration
#   # ############################################################################

#   if(CONFIG_CANUTILS_CANOPENNODE_TOOLS_COCTL)
#     nuttx_add_application(
#       NAME
#       coctl
#       PRIORITY
#       ${CONFIG_CANUTILS_CANOPENNODE_TOOLS_COCTL_PRIORITY}
#       STACKSIZE
#       ${CONFIG_CANUTILS_CANOPENNODE_TOOLS_COCTL_STACKSIZE}
#       COMPILE_FLAGS
#       ${CFLAGS}
#       SRCS
#       ${CANOPENNODE_DIR}/tools/coctl.c
#       MODULE
#       ${CONFIG_CANUTILS_CANOPENNODE}
#       INCLUDE_DIRECTORIES
#       ${INCDIR}
#       DEPENDS
#       lelycanopen)
#   endif()

#   # ############################################################################
#   # Library Configuration
#   # ############################################################################

#   target_compile_options(lelycanopen PRIVATE ${CFLAGS})
#   target_sources(lelycanopen PRIVATE ${CSRCS})
#   target_include_directories(lelycanopen PRIVATE ${INCDIR})

# endif()
