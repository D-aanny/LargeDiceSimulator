project(
    'LargeDiceSimulator',
    'cpp',
    version: '1.0.0',
    default_options: [
        'cpp_std=c++20',
    ],
)

qt6 = import('qt6')

qt6_dep = dependency(
    'qt6',
    modules: ['Core', 'Gui', 'Widgets'],
)

moc_mainwindow = qt6.compile_moc(
    headers: 'src/mainwindow.h',
)

executable(
    'LargeDiceSimulator',
    [
        'src/main.cpp',
        'src/mainwindow.cpp',
        'src/mainwindow.h',
        'src/dicesimulator.cpp',
        'src/dicesimulator.h',
        moc_mainwindow,
    ],
    dependencies: qt6_dep,
    install: true,
)
