#!/usr/bin/env python
# -*- coding: utf-8 -*-

from json import loads
from subprocess import Popen
from socket import gethostname
from argparse import ArgumentParser
from os.path import join, exists, dirname, abspath
from os import curdir, environ, makedirs, removedirs

manifest = loads(file('project.json').read())
host = gethostname()
options = manifest['options']
builders = manifest['builders']
env = (manifest['env'][host] if host in manifest['env'] else {})


def generate(builder, project, debug):

    src = join(dirname(abspath(curdir)), project)

    if not exists(src):
        raise NameError('project "{0}" source not found in {1}'.format(project, src))

    generator = (builder if builder not in builders else builders[builder])
    builddir = join(join(curdir, builder), project)
    cmake_opts = ['cmake', '-G', generator]
    cmake_opts.append('-DCMAKE_MODULE_PATH={0}'.format(abspath(curdir)))

    if debug:
        cmake_opts.append('-DCMAKE_BUILD_TYPE=Debug')
    else:
        cmake_opts.append('-DCMAKE_BUILD_TYPE=Release')

    for option in options:
        applies_to_builder = 'builders' in option and builder in option['builders']
        applies_to_project = 'projects' in option and project in option['projects']
        if applies_to_project or applies_to_builder:
            command = (option['command'] if 'command' in option else None)
            if 'env' in option:
                name = option['env']
                if name in environ:
                    value = environ[name]
                elif name in env:
                    value = env[name]
                else:
                    value = None
                if value:
                    command = '-D{0}={1}'.format(name, value)

            if command:
                cmake_opts.append(command)

    cmake_opts.append(src)
    return (builddir, cmake_opts)


if __name__ == '__main__':
    try:
        parser = ArgumentParser()
        parser.add_argument('--clean', action='store_true')
        parser.add_argument('--show', action='store_true')
        parser.add_argument('--test', action='store_true')
        parser.add_argument('--debug', action='store_true')
        parser.add_argument('--project', required=True)
        parser.add_argument('--builder', required=True)
        args = parser.parse_args()
        (builddir, commands) = generate(args.builder, args.project, args.debug)
        if args.clean:
            if args.show:
                print 'rm -rf {0}'.format(builddir)
            else:
                import shutil
                shutil.rmtree(builddir)
        else:
            if args.show:
                print 'mkdir {0}'.format(builddir)
                print ' '.join(commands)
            else:
                if not exists(builddir):
                    makedirs(builddir)
                Popen(commands, cwd=builddir).wait()
    except Exception as e:
        print 'Error: {0}'.format(str(e))
