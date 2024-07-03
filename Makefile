.PHONY: build clean rebuild test retest

build:
	pip install .

clean:
	rm -rf build
	rm -rf src/*.egg-info

rebuild: clean build

test:
	python -m unittest discover -s test -p "*.py" -t .

retest: rebuild test