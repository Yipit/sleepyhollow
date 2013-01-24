# Sleepy Hollow
> version 0.1.8

![http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg](http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg)

This is our exclusive badass headless browser aimed on what we do
best: scraping.


## Rationale

We need to scrape websites after processing CSS and JS, therefore we
need a full-stack browser and yet, we need it to be headless.

## Production notes

When running production make sure to export the environment variable:

```shell
export QT_QPA_PLATFORM=minimal
```

It will ensure that QT5 will load smoothly on GNU/Linux

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
