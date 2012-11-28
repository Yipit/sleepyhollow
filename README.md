# Sleepy Hollow

![http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg](http://f.cl.ly/items/0O350E410p1Y2w18292F/sleepy-hollow.jpg)

This is our exclusive badass headless browser aimed on what we do
best: scraping.


## Rationale

We need to scrape websites after processing CSS and JS, therefore we
need a full-stack browser and yet, we need it to be headless.

## Hacking

### install dependencies


1. Install [QT](http://qt-project.org/doc/qt-4.8/qtwebkit.html), the core library for the browser

```console
brew upgrade
brew install qt
```

2. Install [SIP](http://riverbankcomputing.co.uk/static/Docs/sip4/index.html), a library that leverages python bindings to C++

```console
brew install sip
```

3. Install the python dependencies

```console
mkvirtualenv sleepy-hollow
make dependencies
```

### build it !

The command below will build the module and run tests against it.

```console
make test
```
