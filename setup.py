#
# This file is written based on benjaminjack/python_cpp_example
# https://github.com/benjaminjack/python_cpp_example
#
# Copyright (c) 2017 Benjamin R. Jack
#

import os
import re
import sys
import sysconfig
import platform
import subprocess

from distutils.version import LooseVersion
from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext

import glob

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='.'):
        Extension.__init__(self, name, sources=glob.glob(sourcedir + "/**/*", recursive=True))
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                    "CMake must be installed to build the following extensions: " +
                    ", ".join(e.name for e in self.extensions))

            if platform.system() == "Windows":
                cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                    out.decode()).group(1))
                if cmake_version < '3.1.0':
                    raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
                os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                cfg.upper(),
                extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
                env.get('CXXFLAGS', ''),
                self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                cwd=self.build_temp)
        print()  # Add an empty line for cleaner output

with open('README.md', encoding='utf-8') as f:
    long_description = f.read()

setup(
        name='servoarray',
        version='0.6.1',
        author='coord.e',
        author_email='me@coord-e.com',
        description='ServoArray: Pluggable servomotor driver library',
        long_description=long_description,
        long_description_content_type='text/markdown',
        ext_modules=[CMakeExtension('servoarray', './src/')],
        cmdclass=dict(build_ext=CMakeBuild),
        zip_safe=False,
        classifiers=[
            'Development Status :: 4 - Beta',
            'Intended Audience :: Developers',
            'Environment :: Other Environment',
            'License :: OSI Approved :: GNU General Public License v2 (GPLv2)',
            'Operating System :: POSIX',
            'Programming Language :: Python',
            'Programming Language :: C++',
            'Topic :: Software Development :: Libraries',
            'Topic :: Software Development :: Embedded Systems',
            'Topic :: System :: Hardware :: Hardware Drivers',
            ],
        )

