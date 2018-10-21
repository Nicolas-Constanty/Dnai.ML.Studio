import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
    property alias background: _page.background
    property alias contentChildren : _page.contentChildren
    property alias contentData : _page.contentData
    property alias contentHeight : _page.contentHeight
    property alias contentWidth : _page.contentWidth
    property alias footer : _page.footer
    property alias header : _page.header
    property alias title : _page.title
    property Item next
    property Item nextButton
    Page {
        id: _page
        anchors.fill: parent
        anchors.margins: 15
        onBackgroundChanged: {
            background.anchors.fill = background.parent
            background.anchors.margins = -15
        }
        footer: Item {
            height: 40
        }
    }
    onNextButtonChanged: {
        if (nextButton)
        {
            _page.footer.children.push(nextButton)
            nextButton.anchors.right = nextButton.parent.right
            nextButton.anchors.bottom = nextButton.parent.bottom
        }
    }
}


