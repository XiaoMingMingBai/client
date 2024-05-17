from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.errors import ConanInvalidConfiguration

class CompressorRecipe(ConanFile):
    name = "compressor"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("glog/0.7.0")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.os == "Macos" and self.settings.arch == "armv8":
            raise ConanInvalidConfiguration("ARM v8 not supported in Macos")