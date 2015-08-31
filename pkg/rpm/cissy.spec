Summary: Cissy
Name: cissy
Version: master
Release: 1
License: Apache 2.0
Group: Applications/System
BuildRoot: %{_builddir}/%{name}-root 
URL: http://github.com/slass100/cissy
Source: master.tar.gz
Vendor: slass100
Packager: slass100
BuildRequires: gcc

%description
a csv file manipulation utility / tool

%prep
echo 'cissy-prep'
pwd
%setup

%build
echo 'cissy-build'
pwd
cd src
make cissy

%install
echo 'cissy-install'
pwd
cd src
install -s -m 755 cissy $RPM_BUILD_ROOT/usr/bin/cissy
install -m 644 ../doc/man/cissy.1 $RPM_BUILD_ROOT/usr/man/man1/cissy.1

%post
echo "post"

%clean
echo "clean"

%files
%defattr(-,root,root)
/usr/bin/cissy

%preun
echo "preun"

%postun
echo "postun"

%changelog
* Tue Aug 25 2015 - slass100 @ users.noreply.github.com
initial spec file
