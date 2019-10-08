%define _prefix __auto__
%define version __auto__
%define release __auto__

Summary: Syslog for Yorick
Name: yorick-syslog
Version: %{version}
Release: 1%{?dist}.gemini
URL: http://www.gemini.edu
Packager: Matthieu Bec <mbec@gemini.edu>
License: mbec@gemini.edu
Group: Gemini
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: yorick
Requires: yorick

Source0: %{name}-%{version}.tar.gz

%define debug_package %{nil}
%define y_exe_home %(echo Y_HOME  | yorick -q | awk -F '"' '{print $2}')
%define y_exe_site %(echo Y_SITE  | yorick -q | awk -F '"' '{print $2}')

%description
Syslog client wrapper for yorick.

%prep
%setup -q -n %name

%build
gmake -f yorick-syslog.make1st
gmake

%install
mkdir -p  $RPM_BUILD_ROOT/%y_exe_home/lib $RPM_BUILD_ROOT/%y_exe_site/i-start $RPM_BUILD_ROOT/%y_exe_site/i0
cp syslog.so $RPM_BUILD_ROOT/%y_exe_home/lib
cp syslog_start.i $RPM_BUILD_ROOT/%y_exe_site/i-start
cp syslog.i $RPM_BUILD_ROOT/%y_exe_site/i0

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%y_exe_home/lib/syslog.so
%y_exe_site/i-start/syslog_start.i
%y_exe_site/i0/syslog.i

%changelog
* Tue Jan 12 2010 Matthieu Bec <mbec@gemini.edu> 0.1-1
- first spec
