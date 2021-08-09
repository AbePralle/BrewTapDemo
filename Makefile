INSTALL_FOLDER=.

all: homebrew

homebrew:
	@# Executable folder should be called "bin/" because Homebrew automatically
	@# creates symlinks in /usr/local to files that you copy into bin/ during
	@# installation.
	@echo "Installing brewtapdemo to $(INSTALL_FOLDER)"
	mkdir -p $(INSTALL_FOLDER)/bin
	mkdir -p $(INSTALL_FOLDER)/Data
	$(CXX) Source/BrewTapDemo.cpp -o $(INSTALL_FOLDER)/bin/brewtapdemo
	echo "Hello World!" > $(INSTALL_FOLDER)/Data/Greeting.txt

clean:
	rm -rf $(INSTALL_FOLDER)/bin
	rm -rf $(INSTALL_FOLDER)/Data

