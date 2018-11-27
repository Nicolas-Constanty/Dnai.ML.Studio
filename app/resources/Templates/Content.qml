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
    property Item nextButton
    property Item prevButton

    Page {
        id: _page
        anchors.fill: parent
        anchors.margins: 30
        onBackgroundChanged: {
            background.anchors.fill = background.parent
            background.anchors.margins = -30
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

    onPrevButtonChanged: {
        if (prevButton)
        {
            _page.footer.children.push(prevButton)
            prevButton.anchors.left = prevButton.parent.left
            prevButton.anchors.bottom = prevButton.parent.bottom
        }
    }
}


