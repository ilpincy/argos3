%define beta beta29

Summary:       A fast, parallel, multi-physics, and multi-robot simulator
Name:          argos3_simulator
Version:       3.0.0%{beta}
Release:       0
License:       MIT
Group:         Development/Tools
Source:        argos3-3.0.0-%{beta}.tar.gz
URL:           http://www.argos-sim.org/
Packager:      Carlo Pinciroli <ilpincy@gmail.com>
BuildRequires: gcc gcc-c++ pkg-config cmake gsl-devel freeglut-devel libqt4-devel freeimage-devel lua51-devel graphviz-gnome graphviz doxygen
BuildRoot:     %{_tmppath}/%{name}-%{version}-build

%description
ARGoS (multi-physics multi-robot simulator)
ARGoS is a highly scalable multi-robot simulator. Among its distinctive
features, there are: (i) modularity (robots, devices, physics engines,
visualizations and controllers are plug-ins), (ii) tunable accuracy, and (iii)
the possibility to run multiple physics engines at the same time.

%package devel
Summary:       ARGoS3 development package
Group:         Development/Tools
BuildRequires: pkg-config
Requires:      %name = %version

%description devel
This package contains the headers files necessary to develop robot controllers in C++.

%package doc
Summary:  ARGoS3 documentation package
Group:    Development/Tools
Requires: %name = %version

%description doc
This package contains the API documentation of ARGoS3.

%prep
%setup -n argos3-3.0.0-%{beta}

%build
rm -rf doc/api/standalone
mkdir -p build
pushd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%{_prefix} ../src
make %{?_smp_mflags}
make doc
popd
rm -f doc/api/standalone/*.md5

%install
pushd build
make DESTDIR=$RPM_BUILD_ROOT install
popd
mkdir -p $RPM_BUILD_ROOT%{_docdir}
mv $RPM_BUILD_ROOT%{_prefix}/doc/argos3 $RPM_BUILD_ROOT%{_docdir}

%files
%defattr(-,root,root)
%config %{_sysconfdir}/ld.so.conf.d/argos3.conf
%doc %{_mandir}/man1/argos3.1.gz
%{_bindir}/argos3
%{_prefix}/lib/argos3
%{_datadir}/argos3
%{_datadir}/applications/argos3.desktop
%{_datadir}/mime/packages/argos.xml

%files devel
%defattr(-,root,root)
%{_includedir}/argos3
%{_prefix}/lib/pkgconfig/argos3_simulator.pc

%files doc
%defattr(-,root,root)
%docdir %{_docdir}/argos3
%{_docdir}/argos3

%post
/sbin/ldconfig
update-mime-database "%{_datadir}/mime/" 2> /dev/null
[ -x update-desktop-database ] && update-desktop-database -q 2> /dev/null
[ -d %{_sysconfdir}/bash_completion.d ] && ln -sf %{_datadir}/argos3/argos3_bash_completion.sh %{_sysconfdir}/bash_completion.d/argos3

%postun
/sbin/ldconfig
update-mime-database "%{_datadir}/mime/" 2> /dev/null
[ -x update-desktop-database ] && update-desktop-database -q 2> /dev/null
[ -d %{_sysconfdir}/bash_completion.d ] && rm -f %{_sysconfdir}/bash_completion.d/argos3

%changelog
