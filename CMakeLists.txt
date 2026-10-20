CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(noti-list-svc C)

SET(PKGNAME "org.tizen.noti-list-svc")
SET(BINDIR "${TZ_SYS_RO_APP}/${PKGNAME}/bin")

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED
	capi-appfw-service-application
	notification
	dlog
	capi-base-common
)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -fPIE -Wall -Werror")

ADD_EXECUTABLE(${PROJECT_NAME} src/main.c)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS} -pie)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
INSTALL(FILES ${PKGNAME}.xml DESTINATION ${TZ_SYS_RO_PACKAGES})
