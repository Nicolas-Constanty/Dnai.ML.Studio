TEMPLATE = subdirs
CONFIG += object_parallel_to_source

SUBDIRS = app qtplugins libs

app.file = app/app.pro
qtplugins.file = plugins/plugins.pro
libs.file = libs/libs.pro

app.depends = qtplugins libs
