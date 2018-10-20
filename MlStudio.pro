TEMPLATE = subdirs
CONFIG += object_parallel_to_source

SUBDIRS = app qtplugins

app.file = app/app.pro
qtplugins.file = plugins/plugins.pro

app.depends = qtplugins