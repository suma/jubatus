import Options
from functools import partial
from waflib.Errors import TaskNotReady
import os
import sys

VERSION = '0.4.5'
APPNAME = 'jubatus'
CORENAME = 'jubatus_core'

top = '.'
out = 'build'
subdirs = ['jubatus']

def options(opt):
  opt.load('compiler_cxx')
  opt.load('unittest_gtest')
  opt.load('gnu_dirs')

  opt.add_option('--enable-debug',
                 action='store_true', default=False,
                 dest='debug', help='build for debug')

  opt.add_option('--enable-gcov',
                 action='store_true', default=False,
                 dest='gcov', help='only for debug')

  opt.recurse(subdirs)

def configure(conf):
  conf.env.CXXFLAGS += ['-O2', '-Wall', '-g', '-pipe']

  conf.load('compiler_cxx')
  conf.load('unittest_gtest')
  conf.load('gnu_dirs')

  # Generate config.hpp
  conf.env.JUBATUS_PLUGIN_DIR = conf.env['LIBDIR'] + '/jubatus/plugin'
  conf.define('JUBATUS_VERSION', VERSION)
  conf.define('JUBATUS_APPNAME', APPNAME)
  conf.define('JUBATUS_PLUGIN_DIR', conf.env.JUBATUS_PLUGIN_DIR)
  conf.write_config_header('jubatus/config.hpp', guard="JUBATUS_CONFIG_HPP_", remove=False)

  conf.check_cxx(lib = 'msgpack')
  conf.check_cxx(lib = 'dl')

  # pkg-config tests
  conf.find_program('pkg-config') # make sure that pkg-config command exists
  try:
    conf.check_cfg(package = 'libglog', args = '--cflags --libs')
    conf.check_cfg(package = 'pficommon', args = '--cflags --libs')
  except conf.errors.ConfigurationError:
    e = sys.exc_info()[1]
    conf.to_log("PKG_CONFIG_PATH: " + os.environ.get('PKG_CONFIG_PATH', ''))
    conf.fatal("Failed to find the library. Please confirm that PKG_CONFIG_PATH environment variable is correctly set.", e)

  conf.check_cxx(header_name = 'dlfcn.h')

  if not Options.options.debug:
    conf.define('NDEBUG', 1)

  if Options.options.gcov:
    conf.env.append_value('CXXFLAGS', '-fprofile-arcs')
    conf.env.append_value('CXXFLAGS', '-ftest-coverage')
    conf.env.append_value('LINKFLAGS', '-lgcov')

  conf.define('BUILD_DIR',  conf.bldnode.abspath())

  conf.recurse(subdirs)

def build(bld):

  bld(name = 'core_headers', export_includes = './')

  def add_prefix(bld, paths):
    prefix_dir = os.path.dirname(bld.cur_script.relpath())
    return [os.path.join(prefix_dir, str(path)) for path in paths]

  bld.add_prefix = partial(add_prefix, bld)
  bld.core_sources = []
  bld.core_headers = []
  bld.core_use = []
  bld.recurse(subdirs)

  bld.shlib(source=list(set(bld.core_sources)), target='jubatus_core', use=list(set(bld.core_use)), vnum = bld.env['JUBATUS_VERSION'])
  bld.install_files('${PREFIX}/include/', list(set(bld.core_headers)), relative_trick=True)

  bld(source = 'jubatus_core.pc.in',
      prefix = bld.env['PREFIX'],
      exec_prefix = '${prefix}',
      libdir = bld.env['LIBDIR'],
      includedir = '${prefix}/include',
      PACKAGE = CORENAME,
      VERSION = VERSION)

def cpplint(ctx):
  import fnmatch, tempfile
  cpplint = ctx.path.find_node('tools/codestyle/cpplint/cpplint.py')
  src_dir = ctx.path.find_node('jubatus')
  file_list = []
  excludes = []
  for file in src_dir.ant_glob('**/*.cpp **/*.cc **/*.hpp **/*.h'):
    file_list += [file.path_from(ctx.path)]
  for exclude in excludes:
    file_list = [f for f in file_list if not fnmatch.fnmatch(f, exclude)]
  tmp_file = tempfile.NamedTemporaryFile(delete=True);
  tmp_file.write("\n".join(file_list));
  tmp_file.flush()
  ctx.exec_command('cat ' + tmp_file.name + ' | xargs "' + cpplint.abspath() + '" --filter=-runtime/references,-runtime/rtti 2>&1')
  tmp_file.close()

