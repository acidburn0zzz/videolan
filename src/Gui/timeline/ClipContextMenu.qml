import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Menu {
    id: clipContextMenu
    title: "Edit"

    property var clip
    property bool grouped
    property bool linked

    MenuItem {
        text: grouped ? "Ungroup" : "Group"

        onTriggered: {
            if ( selectedClips.length <= 1 )
                return;

            if ( grouped === true ) {
                removeGroup( clip.uuid );
            }
            else {
                var l = [];
                for ( var i = 0; i < selectedClips.length; ++i ) {
                    l.push( "" + selectedClips[i].uuid );
                }
                addGroup( l );
            }
        }
    }

    MenuItem {
        id: linkOrUnlink
        text: linked ? "Unlink" : "Link"

        onTriggered: {
            if ( linked === true ) {
                for ( var i = 0; i < selectedClips.length; ++i ) {
                    for ( var j = i + 1; j < selectedClips.length; ++j ) {
                        if ( selectedClips[i].linkedClips.indexOf( selectedClips[j].uuid ) !== -1 )
                            workflow.unlinkClips( selectedClips[i].uuid, selectedClips[j].uuid );
                    }
                }
            }
            else {
                for ( i = 0; i < selectedClips.length; ++i ) {
                    for ( j = i + 1; j < selectedClips.length; ++j ) {
                        if ( selectedClips[i].linkedClips.indexOf( selectedClips[j].uuid ) === -1 )
                            workflow.linkClips( selectedClips[i].uuid, selectedClips[j].uuid );
                    }
                }
            }
        }
    }

    MenuSeparator { }

    MenuItem {
        text: "Delete"

        onTriggered: {
            removeClipDialog.visible = true;
        }
    }

    MessageDialog {
        id: removeClipDialog
        title: "VLMC"
        text: qsTr( "Do you really want to remove the clip?" )
        icon: StandardIcon.Question
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            workflow.removeClip( clip.uuid );
        }
    }

    onAboutToShow: {
        linked = clip.linked();
        grouped = findGroup( clip.uuid );
        linkOrUnlink.enabled = selectedClips.length > 1
    }

    MenuSeparator { }

    MenuItem {
        text: "Effects"

        onTriggered: {
            workflow.showEffectStack( clip.uuid );
        }
    }
}
