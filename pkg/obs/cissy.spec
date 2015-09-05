
# Build File for RPM based distros

Summary: Cissy
Name: cissy
Version: 0.3
Release: 1
License: Apache 2.0
Group: Applications/System
BuildRoot: %{_builddir}/%{name}-root 
URL: http://github.com/slass100/cissy
Source: cissy-v0.3.tar.gz
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
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/man/man1
make install DESTDIR=$RPM_BUILD_ROOT

%post
echo "post"

%clean
echo "clean"

%files
%defattr(-,root,root)
/usr/bin/cissy
/usr/share/man/man1/cissy.1.gz

%preun
echo "preun"

%postun
echo "postun"

%changelog
* Tue Aug 25 2015 - slass100 @ users.noreply.github.com
initial spec file

