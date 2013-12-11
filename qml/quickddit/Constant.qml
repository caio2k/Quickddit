import QtQuick 1.1
import Quickddit 1.0

QtObject {
    id: constant

    property color colorLight: theme.inverted ? "#ffffff" : "#191919"
    property color colorMid: theme.inverted ? "#8c8c8c" : "#666666"
    property color colorDisabled: theme.inverted ? "#444444" : "#b2b2b4"

    property color colorPositive: "green"
    property color colorNegative: "red"
    property color colorNeutral: theme.inverted ? "dimgray" : "darkgray"

    property int paddingSmall: 4
    property int paddingMedium: 8
    property int paddingLarge: 12
    property int paddingXLarge: 16

    property int fontSizeXSmall: 20
    property int fontSizeSmall: 22
    property int fontSizeMedium: 24
    property int fontSizeLarge: 26
    property int fontSizeXLarge: 28
    property int fontSizeXXLarge: 32

    property int fontSizeDefault: __fontSizeDefaultF()

    function __fontSizeDefaultF() {
        switch (appSettings.fontSize) {
        case AppSettings.SmallFontSize: return constant.fontSizeSmall;
        case AppSettings.MediumFontSize: return constant.fontSizeMedium;
        case AppSettings.LargeFontSize: return constant.fontSizeLarge;
        }
    }

    property int headerHeight: inPortrait ? 72 : 56

    // Quickddit specific
    property int commentRepliesIndentWidth: 20
    property variant commentRepliesColor: ["green", "orange", "purple", "yellow",
                                           "royalblue", "pink", "indigo", "gold",
                                           "red", colorLight];
}
