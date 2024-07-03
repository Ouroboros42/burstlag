.PHONY: build clean test

build:
	pip install .

clean:
	rm -rf build

test: build
	python -m unittest discover -s test -p "*.py" -t .