.PHONY: build clean rebuild test retest time retime

build:
	pip install .

clean:
	rm -rf build
	rm -rf src/*.egg-info

rebuild: clean build

test:
	python -m unittest discover -s test -p "*.py" -t .

time:
	python test/timing/simple.py

retest: rebuild test

retime: rebuild time