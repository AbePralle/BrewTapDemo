# BrewTapDemo
Demonstrates how to create a third-party Homebrew Tap + Formulae. Uses a build-from-source C++ demo app.

To install and run this example with Homebrew, execute:

    brew install abepralle/tools/brewtapdemo
    brewtapdemo

The following sections describe how to set up a similar Tap with Formulae.

# Create a New Tap

This will create a new local

    brew tap-new abepralle/tools

My GitHub account is `AbePralle` but just use all lowercase.

    Initialized empty Git repository in /usr/local/Homebrew/Library/Taps/abepralle/homebrew-tools/.git/
    [main (root-commit) 8a2cfc1] Create abepralle/tools tap
     3 files changed, 88 insertions(+)
     create mode 100644 .github/workflows/publish.yml
     create mode 100644 .github/workflows/tests.yml
     create mode 100644 README.md
    ==> Created abepralle/tools
    /usr/local/Homebrew/Library/Taps/abepralle/homebrew-tools

    When a pull request making changes to a formula (or formulae) becomes green
    (all checks passed), then you can publish the built bottles.
    To do so, label your PR as `pr-pull` and the workflow will be triggered.

Edit `~/.bash_profile` or other `.profile` and add something like:

    alias brewtools="cd /usr/local/Homebrew/Library/Taps/abepralle/homebrew-tools"

Create an empty corresponding repo `homebrew-tools` on your GitHub account. Link it to your new local repo with:

    git remote set-url origin https://github.com/AbePralle/homebrew-tools

Then:

    git push

# Create a New Formula

Obtain a link to a `.tar.gz` project that that builds C++ via Makefile or CMake and installs executables to a `bin/` folder. Adjust the makefile so that it installs to `bin/` with an adjustable prefix path such as `INSTALL_FOLDER`. For example:

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

One way to do this is to create a "Release" on a GitHub repo such as this one. The "Releases" page will then automatically have a link to the `.tar.gz` of that latest release. Copy that link.

On Terminal, run:

    brew create https://link-to-your.tar.gz

Brew will create a new .rb (Ruby) *formulae* in Brew's `homebrew-core/Formulas` folder and launch an editor. From your `homebrew-tools` folder, quit the editor and `mv <original-formula-filepath.rb> ./Formulas` to relocate the fomulae to your custom Tap folder. Then edit it and flesh out all the details. For example:

