#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=phidget21-servo-utils
PKG_VERSION:=2.1
PKG_RELEASE:=1

PKG_SOURCE:=phidget-servo-utils.tar
PKG_BUILD_DIR:=$(BUILD_DIR)/st
PKG_SOURCE_URL:=http://localhost/
PKG_MD5SUM:=7af033e14392b7621fa089fbbcc9eabe

include $(INCLUDE_DIR)/package.mk

define Package/phidget21-servo-utils
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Phidget Servo Utilities
  URL:=http://www.phidgets.com/
  DEPENDS:=phidget21
endef

define Package/phidget21-servo-utils/description
 Servo controller programs for the Phidget library.
endef

define Build/Compile
  $(MAKE) -C $(PKG_BUILD_DIR) \
  	$(TARGET_CONFIGURE_OPTS) \
  	LIBS="" \
  	CFLAGS="$(TARGET_CFLAGS) -D_LINUX -I." \
  	all
endef

define Package/phidget21-servo-utils/install
  $(INSTALL_DIR) $(1)/usr/bin
  $(CP) $(PKG_BUILD_DIR)/servo $(1)/usr/bin/
endef

$(eval $(call BuildPackage,phidget21-servo-utils))

