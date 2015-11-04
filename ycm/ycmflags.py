import os
import ycm_core
from clang_helpers import PrepareClangFlags


class YcmFlags:
    def __init__(self, additional_includes=[], enable_qt=False):
        self.flags = [
            '-std=c++11',
            '-x', 'c++',
            '-I', './',
            '-I', './3rd-party/cpr/include/',
            '-I', './3rd-party/rapidjson/include/',
        ]
        self.project_path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        self.compilation_database_folder = os.path.join(self.project_path, 'build/')
        self.database = None
        if enable_qt:
            self.flags.extend([
                '-D', 'QT_CORE_LIB',
                '-D', 'QT_GUI_LIB',
                '-D', 'QT_NETWORK_LIB',
                '-D', 'QT_WIDGETS_LIB',
            ])
            self.find_qt_components()
        for include in additional_includes:
            self.flags.extend(['-I', include])

    @staticmethod
    def find_qt5_location():
        prefixes = ["/usr/include/qt5", "/usr/include/qt"]
        for prefix in prefixes:
            if os.path.isdir(prefix + '/QtCore'):
                return prefix
        raise Exception('QT include path not found')

    def find_qt_components(self):
        location = self.find_qt5_location()
        components = [d for d in os.listdir(location) if os.path.isdir(os.path.join(location, d))]
        self.flags.extend(['-I', location])
        for component in components:
            inclusion_path = os.path.join(location, component)
            self.flags.extend(['-I', inclusion_path])
        if self.compilation_database_folder:
            self.database = ycm_core.CompilationDatabase(self.compilation_database_folder)
        else:
            self.database = None

    @staticmethod
    def relative_to_absolute(flags, working_directory):
        if not working_directory:
            return flags
        new_flags = []
        make_next_absolute = False
        path_flags = ['-isystem', '-I', '-iquote', '--sysroot=']
        for flag in flags:
            new_flag = flag
            if make_next_absolute:
                make_next_absolute = False
                if not flag.startswith('/'):
                    new_flag = os.path.join(working_directory, flag)
            for path_flag in path_flags:
                if flag == path_flag:
                    make_next_absolute = True
                    break
                if flag.startswith(path_flag):
                    path = flag[len(path_flag):]
                    new_flag = path_flag + os.path.join(working_directory, path)
            if new_flag:
                new_flags.append(new_flag)
        return new_flags

    def flags_for_file(self, filename):
        final_flags = None
        if self.database:
            # Bear in mind that compilation_info.compiler_flags_ does NOT return a
            # python list, but a "list-like" StringVec object
            compilation_info = self.database.GetCompilationInfoForFile(filename)
            final_flags = PrepareClangFlags(
                self.relative_to_absolute(
                    compilation_info.compiler_flags_,
                    compilation_info.compiler_working_dir_),
                filename)
        if not final_flags:
            relative_to = self.project_path
            final_flags = self.relative_to_absolute(self.flags, relative_to)
        return {
            'flags': final_flags,
            'do_cache': True
        }

