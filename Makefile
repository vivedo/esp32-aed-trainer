.PHONY: format
format:
	clang-format --verbose -i **/*.cpp **/*.h
