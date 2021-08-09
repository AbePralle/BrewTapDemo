# BrewTapDemo
Demonstrates how to create a third-party Homebrew Tap and create a Formula to install a C++ demo app.

To install and run the example shown in this repo with Homebrew, execute:

    brew install abepralle/tools/brewtapdemo
    brewtapdemo
    # Prints out "Hello World!"

The following sections describe how to set up a similar Tap with Formulae.

# Create a New Tap

This will create a new local Git repo:

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

Brew will create a new .rb (Ruby) *formula* in Brew's `homebrew-core/Formula` folder:

    > brew create https://github.com/AbePralle/BrewTapDemo/archive/refs/tags/v1.0.tar.gz
    ==> Downloading https://github.com/AbePralle/BrewTapDemo/archive/refs/tags/v1.0.tar.gz
    ==> Downloading from https://codeload.github.com/AbePralle/BrewTapDemo/tar.gz/refs/tags/v1.0
    -#O#- #   #
    Warning: Cannot verify integrity of '8997e132655c5c0da3c2ffa9463edb0191857964db364163be90ec880de226a4--BrewTapDemo-1.0.tar.gz'.
    No checksum was provided for this resource.
    For your reference, the checksum is:
      sha256 "89664b3259b279587c990aa19255b40235648d63dcbe35d010a770eb47061856"
    Please run `brew audit --new BrewTapDemo` before submitting, thanks.
    Editing /usr/local/Homebrew/Library/Taps/homebrew/homebrew-core/Formula/brewtapdemo.rb

And launch an editor:

    # Documentation: https://docs.brew.sh/Formula-Cookbook
    #                https://rubydoc.brew.sh/Formula
    # PLEASE REMOVE ALL GENERATED COMMENTS BEFORE SUBMITTING YOUR PULL REQUEST!
    class Brewtapdemo < Formula
      desc "Demonstrates how to create a third-party Homebrew Tap + Formula"
      homepage ""
      url "https://github.com/AbePralle/BrewTapDemo/archive/refs/tags/v1.0.tar.gz"
      sha256 "89664b3259b279587c990aa19255b40235648d63dcbe35d010a770eb47061856"
      license "MIT"

      # depends_on "cmake" => :build

      def install
        # ENV.deparallelize  # if your formula fails when building in parallel
        # Remove unrecognized options if warned by configure
        # https://rubydoc.brew.sh/Formula.html#std_configure_args-instance_method
        system "./configure", *std_configure_args, "--disable-silent-rules"
        # system "cmake", "-S", ".", "-B", "build", *std_cmake_args
      end

      test do
        # `test do` will create, run in and delete a temporary directory.
        #
        # This test will fail and we won't accept that! For Homebrew/homebrew-core
        # this will need to be a test that verifies the functionality of the
        # software. Run the test with `brew test BrewTapDemo`. Options passed
        # to `brew install` such as `--HEAD` also need to be provided to `brew test`.
        #
        # The installed folder is not in the path, so use the entire path to any
        # executables being tested: `system "#{bin}/program", "do", "something"`.
        system "false"
      end
    end

Quit the editor and move the listed `.rb` file to your `homebrew-tools/Formula` folder:

    cd /usr/local/Homebrew/Library/Taps/abepralle/homebrew-tools
    mv /usr/local/Homebrew/Library/Taps/homebrew/homebrew-core/Formula/brewtapdemo.rb ./Formula/

(If you leave it in the `homebrew-core` folder, it will come up with a "no bottles" error related to installing from source when you try to install it later.)

Then edit it and flesh out all the details. For example:

    class Brewtapdemo < Formula
      desc "Demonstrates how to create a third-party Homebrew Tap + Formula"
      homepage "https://github.com/AbePralle"
      url "https://github.com/AbePralle/BrewTapDemo/archive/refs/tags/v1.0.tar.gz"
      sha256 "89664b3259b279587c990aa19255b40235648d63dcbe35d010a770eb47061856"
      license "MIT"

      depends_on "make" => :build

      def install
        system "make", "homebrew", "INSTALL_FOLDER=#{prefix}"
      end

      test do
        assert_equal "Hello World!", shell_output("#{bin}/brewtapdemo").strip
      end
    end

Test out your new formula with something like:

    brew install abepralle/tools/brewtapdemo
    brewtapdemo
    brew test brewtapdemo

No news is good news with `brew test`; you'll only see an error message if the test fails.

    brew test brewtapdemo
    ==> Testing abepralle/tools/brewtapdemo
    ==> /usr/local/Cellar/brewtapdemo/1.0/bin/brewtapdemo

Finally add the new formula to your repo:

    git add Formula/brewtapdemo.rb
    git commit -am "brewtapdemo"
    git push

You're all set! Now anyone on Mac or Linux can install and use your software with e.g. `brew install abepralle/tools/brewtapdemo`!

