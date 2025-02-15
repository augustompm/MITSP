.PHONY: all clean install build rebuild debug release

BUILD_DIR = build
CMAKE = cmake
CMAKE_BUILD_TYPE ?= Release

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && $(CMAKE) -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ..
	@$(CMAKE) --build $(BUILD_DIR) --config $(CMAKE_BUILD_TYPE)

rebuild: clean build

debug:
	@$(MAKE) BUILD_DIR=build-debug CMAKE_BUILD_TYPE=Debug build

release:
	@$(MAKE) BUILD_DIR=build-release CMAKE_BUILD_TYPE=Release build

clean:
	@rm -rf $(BUILD_DIR) build-debug build-release

install: build
	@$(CMAKE) --install $(BUILD_DIR)

run: build
	@./$(BUILD_DIR)/bin/mitsp