Name:       org.tizen.noti-list-svc
Summary:    Service app to check notification_get_list() on platform build
Version:    1.0.0
Release:    1
Group:      Application Framework/Notifications
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: %{name}.manifest

BuildRequires:  cmake
BuildRequires:  hash-signer
BuildRequires:  pkgconfig(libtzplatform-config)
BuildRequires:  pkgconfig(capi-appfw-service-application)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(notification)
BuildRequires:  pkgconfig(dlog)

%description
Platform-built native service application that calls notification_get_list()
on create and prints every notification and the total count to dlog.

%prep
%setup -q
cp %{SOURCE1001} .

%build
%cmake . -DTZ_SYS_RO_APP=%{TZ_SYS_RO_APP} \
	-DTZ_SYS_RO_PACKAGES=%{TZ_SYS_RO_PACKAGES}
%__make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%make_install

%define tizen_sign 1
%define tizen_sign_base %{TZ_SYS_RO_APP}/%{name}
%define tizen_sign_level platform
%define tizen_author_sign 1
%define tizen_dist_sign 1

%files
%manifest %{name}.manifest
%license LICENSE
%{TZ_SYS_RO_APP}/%{name}/bin/noti-list-svc
%{TZ_SYS_RO_APP}/%{name}/author-signature.xml
%{TZ_SYS_RO_APP}/%{name}/signature1.xml
%{TZ_SYS_RO_PACKAGES}/%{name}.xml
