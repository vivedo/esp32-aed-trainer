CLANG_FORMAT := clang-format --verbose

.PHONY: format
format:
	@find src \( -name "*.cpp" -o -name "*.h" \) -exec $(CLANG_FORMAT) -i {} +
	@echo
	@find lib \( -name "*.cpp" -o -name "*.h" \) -exec $(CLANG_FORMAT) -i {} +
	@echo
	@find test \( -name "*.cpp" -o -name "*.h" \) -exec $(CLANG_FORMAT) -i {} +

.PHONY: encode-audio
encode-audio:
	@scripts/encode_audio.sh