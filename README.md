# Sleepy Hollow
> version 0.1.8

![http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg](http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg)

This is our exclusive badass headless browser aimed on what we do
best: scraping.


## Rationale

We need to scrape websites after processing CSS and JS, therefore we
need a full-stack browser and yet, we need it to be headless.

## Production notes

### The QPA platform

QT has something called QT Platform Application, it basically says
what are the available platforms for QT: cocoa, windows, X11

That can be set through the environment variable `QT_QPA_PLATFORM`.

When running sleepyhollow in production server we want to make QT work
in a way that is compatible with embedded systems which is not much of
a different than we hav ein our amazon instances.

#### Setting up the environment variable

Make sure to export `QT_QPA_PLATFORM` as `minimal`, like below:

```shell
export QT_QPA_PLATFORM=minimal
```


### Fonts

When running in `minimal` (see "QPA Platform" above), QT doesn't know
where to find fonts and therefore type rendering wouldn't work.

As it turns out, the `minimal` backend will look for fonts in
`/path-to-qt5-installation/lib/fonts`

All it requires is a basic set of fonts, most of which can be solved
by using a [single font file](http://unifoundry.com/unifont.html) that contains all the glyphs for all the
existing languages, it can be downloaded from the
[Unifoundry](http://unifoundry.com/) website.

Although, we have our own font package available, ready to be put in
the QT font installation dir, it's available at:
[yipit-software-packages/qt5/qt-fonts.tar.bz2](https://s3.amazonaws.com/yipit-software-packages/qt5/qt-fonts.tar.bz2)

## Hacking

### install dependencies

#### 1. Install [QT5](http://qt-project.org/wiki/Building_Qt_5_from_Git), the core library for the browser

```bash
brew upgrade
brew tap yipit/yipit
brew install yipit/yipit/qt5
```

#### 2. Install build dependencies

```bash
brew install autogen automake autoconf
```

#### 3. Create a virtual env and install dependencies

```bash
mkvirtualenv sleepy-hollow
pip install -r requirements.pip
```

#### 4. Use the `sleepy` command to test it

```bash
./hollow test
```

or if you want to debug the output, use the `--verbose` mode:

```bash
./hollow -v test
./hollow --verbose test
```

### distributing

```bash
./hollow release
```

### running the test server manually

Sleepy Hollow test suite comes with a builtin tornado server, you can
run it with

```bash
./hollow server 4000
```

![http://f.cl.ly/items/1j030T0w1J2V1O171F0N/Screen%20Shot%202012-12-03%20at%201.22.52%20PM.png](http://f.cl.ly/items/1j030T0w1J2V1O171F0N/Screen%20Shot%202012-12-03%20at%201.22.52%20PM.png)
