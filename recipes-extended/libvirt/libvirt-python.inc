inherit pythonnative python-dir

export STAGING_INCDIR
export STAGING_LIBDIR
export BUILD_SYS 
export HOST_SYS

RDEPENDS_${PN}-python += "python"
PACKAGECONFIG_${PN}-python[xen] = ",,,xen-python"

PACKAGES += "${PN}-python-staticdev ${PN}-python-dev ${PN}-python-dbg ${PN}-python"

FILES_${PN}-python-staticdev += "${PYTHON_SITEPACKAGES_DIR}/*.a"
FILES_${PN}-python-dev += "${PYTHON_SITEPACKAGES_DIR}/*.la"
FILES_${PN}-python-dbg += "${PYTHON_SITEPACKAGES_DIR}/.debug/"
FILES_${PN}-python = "${bindir}/* ${libdir}/* ${libdir}/${PYTHON_DIR}/*"

SRC_URI += "http://libvirt.org/sources/python/libvirt-python-${PV}.tar.gz;name=libvirt_python"
SRC_URI += "file://libvirt_api_xml_path.patch;patchdir=../libvirt-python-${PV}"

SRC_URI[libvirt_python.md5sum] = "cd3b75c0eb64e933f0b648fbdca6e868"
SRC_URI[libvirt_python.sha256sum] = "94d79d662b6010f6c688b8856b2d95077736d48ffa35feed5a339d7fafbf8c39"

export LIBVIRT_API_PATH = "${S}/docs/libvirt-api.xml"
export LIBVIRT_CFLAGS = "-I${S}/include"
export LIBVIRT_LIBS = "-L${S}/src/.libs -lvirt -ldl"
export LDFLAGS="-L${S}/src/.libs"

LIBVIRT_INSTALL_ARGS = "--root=${D} \
    --prefix=${prefix} \
    --install-lib=${PYTHON_SITEPACKAGES_DIR} \
    --install-data=${datadir}"

python __anonymous () {
    pkgconfig = d.getVar('PACKAGECONFIG', True)
    if ('python') in pkgconfig.split():
        d.setVar('LIBVIRT_PYTHON_ENABLE', '1')
    else:
        d.setVar('LIBVIRT_PYTHON_ENABLE', '0')
}

do_compile_append() {
	if [ "${LIBVIRT_PYTHON_ENABLE}" = "1" ]; then
		cd ${WORKDIR}/libvirt-python-${PV} && \
		  ${STAGING_BINDIR_NATIVE}/python-native/python setup.py build
	fi
}

do_install_append() {
	if [ "${LIBVIRT_PYTHON_ENABLE}" = "1" ]; then
		cd ${WORKDIR}/${PN}-python-${PV} && \
		  ${STAGING_BINDIR_NATIVE}/python-native/python setup.py install \
                       --install-lib=${D}/${PYTHON_SITEPACKAGES_DIR} ${LIBVIRT_INSTALL_ARGS}
	fi
}
