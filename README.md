# Sleepy Hollow

![http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg](http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg)

This is our exclusive badass headless browser aimed on what we do
best: scraping.


## Rationale

We need to scrape websites after processing CSS and JS, therefore we
need a full-stack browser and yet, we need it to be headless.

## Hacking

### install dependencies

#### 1. Install [QT](http://qt-project.org/doc/qt-4.8/qtwebkit.html), the core library for the browser

```bash
brew upgrade
brew install qt autogen automake autoconf
```

#### 2. Create a virtual env and install dependencies

```bash
mkvirtualenv sleepy-hollow
pip install -r requirements.pip
```

#### 3. Use the `sleepy` command to test it

```bash
./sleepy test
```

or if you want to debug the output, use the `--verbose` mode:

```bash
./sleepy -v test
./sleepy --verbose test
```

### distributing

```bash
./sleepy release
```

### running the test server manually

Sleepy Hollow test suite comes with a builtin tornado server, you can
run it with

```bash
./sleepy server 4000
```

![http://f.cl.ly/items/1j030T0w1J2V1O171F0N/Screen%20Shot%202012-12-03%20at%201.22.52%20PM.png](http://f.cl.ly/items/1j030T0w1J2V1O171F0N/Screen%20Shot%202012-12-03%20at%201.22.52%20PM.png)
