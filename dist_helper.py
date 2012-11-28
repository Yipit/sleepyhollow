# -*- coding: utf-8 -*-

import os
import sipconfig
import commands
import functools
from distutils.command.build_ext import build_ext as BuildExtBase
from distutils.dep_util import newer_group
from hashlib import sha1


def replace_suffix(path, new_suffix):
    return os.path.splitext(path)[0] + new_suffix


QT_LIBS = commands.getoutput('qmake -query QT_INSTALL_LIBS')
QT_HEADERS = commands.getoutput('qmake -query QT_INSTALL_HEADERS')
QT_INTERNALS = [
    'QtCore',
    'QtGui',
    'QtHelp',
    'QtMultimedia',
    'QtNetwork',
    'QtOpenGL',
    'QtScript',
    'QtScriptTools',
    'QtSql',
    'QtWebKit',
    'QtSvg',
    'QtTest',
    'QtUiToolsQtWebKitQtXml',
    'QtXmlPatterns',
]

QT_HEADER_LIST = [QT_HEADERS] + ['%s/%s' % (QT_HEADERS, x) for x in QT_INTERNALS]
QT_LINKED_LIBS_LIST = ['-framework %s' % x for x in QT_INTERNALS]
QT_COMPILE_ARGS = '-DQT_WEBKIT_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED'.split()
class BuildExtension(BuildExtBase, object):
    description = "Compile sleepy hollow against QT"

    user_options = BuildExtBase.user_options[:]
    user_options = [opt for opt in user_options if not opt[0].startswith("swig")]
    user_options += [
        ('sip-opts=', None,
         "list of sip command line options"),
    ]

    SIP_OPTIONS = []
    cfg = sipconfig.Configuration()

    def _get_sip_output_list(self, sbf):
        """
        Parse the sbf file specified to extract the name of the generated source
        files. Make them absolute assuming they reside in the temp directory.
        """
        for L in file(sbf):
            key, value = L.split("=", 1)
            if key.strip() == "sources":
                out = []
                for o in value.split():
                    out.append(os.path.join(self.build_temp, o))
                return out

        raise RuntimeError("cannot parse SIP-generated '%s'" % sbf)

    def _sip_calc_signature(self):
        sip_bin = self.cfg.sip_bin
        return sha1(open(sip_bin, "rb").read()).hexdigest()

    @property
    def signature_filename(self):
        return os.path.join(self.build_temp, "sip.signature")

    def _sip_output_dir(self):
        return self.build_temp

    def build_extension(self, ext):
        oldforce = self.force

        if not self.force:
            sip_sources = [source for source in ext.sources if source.endswith('.sip')]
            if sip_sources:
                sigfile = self.signature_filename
                if not os.path.isfile(sigfile):
                    self.force = True
                else:
                    old_sig = open(sigfile).read()
                    new_sig = self._sip_calc_signature()
                    if old_sig != new_sig:
                        self.force = True

        BuildExtBase.build_extension(self, ext)

        self.force = oldforce

    def swig_sources(self, sources, extension=None):
        if not self.extensions:
            return

        # Add the SIP include directory to the include path
        if extension is not None:
            extension.include_dirs.append(self.cfg.sip_inc_dir)
            extension.include_dirs.extend(QT_HEADER_LIST)
            extension.library_dirs.append(QT_LIBS)
            extension.extra_link_args.extend(QT_LINKED_LIBS_LIST)
            extension.extra_compile_args.extend(QT_COMPILE_ARGS)
            depends = extension.depends

        # Filter dependencies list: we are interested only in .sip files,
        # since the main .sip files can only depend on additional .sip
        # files. For instance, if a .h changes, there is no need to
        # run sip again.
        depends = [f for f in depends if os.path.splitext(f)[1] == ".sip"]

        # Create the temporary directory if it does not exist already
        if not os.path.isdir(self.build_temp):
            os.makedirs(self.build_temp)

        # Collect the names of the source (.sip) files
        sip_sources = [source for source in sources if source.endswith('.sip')]
        other_sources = [source for source in sources if not source.endswith('.sip')]
        generated_sources = []

        sip_bin = self.cfg.sip_bin

        for sip in sip_sources:
            # Use the sbf file as dependency check
            sipbasename = os.path.basename(sip)
            sbf = os.path.join(self.build_temp, replace_suffix(sipbasename, ".sbf"))
            if newer_group([sip] + depends, sbf) or self.force:
                self._sip_compile(sip_bin, sip, sbf)
                open(self.signature_filename, "w").write(self._sip_calc_signature())
            out = self._get_sip_output_list(sbf)
            generated_sources.extend(out)

        return generated_sources + other_sources

    def _sip_compile(self, sip_bin, source, sbf):
        self.spawn([sip_bin] + self.SIP_OPTIONS + [
            "-c", self.build_temp,
            "-b", sbf,
            "-I", self.cfg.default_sip_dir,
            source
        ])
