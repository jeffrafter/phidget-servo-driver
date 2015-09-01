#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=phidget-servo
PKG_VERSION:=2.1
PKG_RELEASE:=1

PKG_SOURCE:=phidget-servo.tar
PKG_BUILD_DIR:=$(BUILD_DIR)/phidget-servo-driver/src
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install
PKG_SOURCE_URL:=http://0.0.0.0

include $(INCLUDE_DIR)/package.mk

define Package/phidget-servo
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Phidget Servo Utility
  URL:=http://github.com/jeffrafter/phidget-servo-driver
endef

define Package/phidget-servo/description
 Servo controller programs for the Phidget library.
endef

define Build/Compile
  $(MAKE) -C $(PKG_BUILD_DIR) \
  	$(TARGET_CONFIGURE_OPTS) \
  	LIBS="" \
  	CFLAGS="$(TARGET_CFLAGS) -D_LINUX -I." \
  	all
endef

define Package/phidget-servo/install
  $(INSTALL_DIR) $(1)/usr/bin
  $(CP) $(PKG_BUILD_DIR)/servo $(1)/usr/bin/
endef

$(eval $(call BuildPackage,phidget-servo))

