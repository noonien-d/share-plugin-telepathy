/* share-plugin-telepathy - a share provider for Sailfish OS.
 * Copyright (C) 2016  Ferdinand Stehle
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation;
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, see <http://www.gnu.org/licenses/>. */

import QtQuick 2.6

import Sailfish.Silica 1.0
import Sailfish.Contacts 1.0
import org.nemomobile.contacts 1.0
import Sailfish.TransferEngine 1.0

SilicaFlickable {
    id : root

    property url source
    property string contactid
    property string methodId
    property bool scaleImage : true

    width: Screen.width
    height: Screen.height

    Column {
        width: parent.width
        padding: Theme.paddingLarge

    	PageHeader {
            visible: root.contactid != ""
            title: qsTr("Options")
        }
        Label {
            id: nameLabel
            visible: root.contactid != ""
            width: 480
            text: qsTr("Receipient") + ": " + root.contactid
        }
        TextSwitch {
            id: scaleSwitch
            visible: root.contactid != ""
            text: qsTr("Scale Image")
            checked: true
            onCheckedChanged: {
                root.scaleImage = checked
            }
        }
        Button{
            text: qsTr("Start")
            visible: root.contactid != ""
            width: Theme.buttonWidthMedium
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                shareItem.start();
                shareAction.done();
            }
        }
    }


    ContactBrowser {
        id: contactBrowser
        anchors.fill: parent

        enabled: true

        searchActive: true
        searchableContactProperty: PeopleModel.AccountUriSearchable

        placeholderText: qsTrId("contacts-la-add_people")

        onContactClicked: {
            console.log(contact.id + " '" + contact.firstName +"' '"+ contact.lastName + "'")

            for (var i = 0; i < contact.accountDetails.length; ++i) {
                console.log(contact.accountDetails[i]["type"] + "-" + contact.accountDetails[i]["subType"] + "-" + contact.accountDetails[i]["label"] + "-" + contact.accountDetails[i]["accountUri"]);

                if (contact.accountDetails[i]["type"] === Person.OnlineAccountType)
                {
                    root.contactid = contact.accountDetails[i]["accountUri"];
                    contactBrowser.visible = false;
                }
            }
        }
    }
    property var shareAction
    Component.onCompleted: {
        shareItem.loadConfiguration(shareAction.toConfiguration())
    }
    SailfishTransfer
    {
        id: shareItem
        source: root.source
        metadataStripped: true
        userData: {"description": "Telepathy File Transfer", "targetid": root.contactid, "scaleimage": root.scaleImage}
    }
}
