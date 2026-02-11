// swift-tools-version: 6.1
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "webrogueswift",
    targets: [
        .executableTarget(
            name: "main",
            dependencies: [
                "WebrogueGFX"
            ],
        ),
        .target(
            name: "WebrogueGFX",
            dependencies: ["WebrogueGFXRaw"]
        ),
        .target(
            name: "WebrogueGFXRaw",
            linkerSettings: [
                .linkedLibrary(
                    "webroguegfx"
                )
            ]
        ),
    ]
)
