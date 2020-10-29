
.PHONY: build
build:
	@echo "building project"
	@gcc test.c -o test
	@gcc runner.c -o runner
	@sudo chown root:root test
	@sudo chmod 4755 test
	@sudo chown root:root secret.txt
	@sudo chmod 400 secret.txt

.PHONY: run
runner: build
	./runner

.PHONY: test
test: build
	./test
