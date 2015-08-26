Summary: Cissy
Name: cissy
Version: 0.1
Release: 1
License: Apache 2.0
Group: Applications/System
BuildRoot: %{_builddir}/%{name}-root 
URL: http://github.com/slass100/cissy
Vendor: slass100
Packager: slass100
BuildRequires: gcc

%description
a csv file manipulation utility / tool

%prep

%build
pwd
cd %{_sourcedir}/cissy/src
make cissy

%install
pwd
cd %{_sourcedir}/cissy/src
mkdir -p $RPM_BUILD_ROOT/usr/bin
make install DESTDIR=$RPM_BUILD_ROOT

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
