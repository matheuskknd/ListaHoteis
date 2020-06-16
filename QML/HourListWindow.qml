import QtQuick.Controls 2.15	// For buttons and UI elements
import QtQuick.Window 2.15		// Tu use "Window" node
import QtQuick 2.15
import My.Lib 1.0

Window{

	property var hotelList: []
	property Window root: null
	property var searchH: ""

	id: window
	visible: false
	color: "#00000000"
	title: qsTr("Lista Hoteis - Tela 2")

	minimumHeight: 480/2
	minimumWidth: 640/2
	height: 480
	width: 640

	function toggleVisibility(){

		if( !window.visible ){

			setY(Screen.height/2 - height/2)
			setX(Screen.width/ 2 - width/2)
		}

		window.visible = !window.visible
	}

	Rectangle{
		
		id: backGround
		anchors.fill: parent
		color: "#212222"

		Rectangle{

			id: returnButton
			border.width: 0.4
			color: "#00827F"
			radius: 5

			width: window.width * 20.83/100
			height: window.height * 12.5/100

			anchors.left: parent.left
			anchors.top: parent.top
			anchors.leftMargin: 20
			anchors.topMargin: 20

			Label{

				anchors.fill: parent

				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
				color: "#000000"
				text: "Voltar"
			}

			// Turns it into a button
			MouseArea{

				onClicked: window.close()
				anchors.fill: parent

				onPressed: parent.color = "#006E6E"
				onReleased: parent.color = "#00827F"
			}
		}

		Label{

			id: searchLabel
			color: "#ffffff"

			width: window.width - 3 * 20 - returnButton.width
			height: window.height * 12.5/100

			anchors.right: parent.right
			anchors.top: parent.top
			anchors.rightMargin: 20
			anchors.topMargin: 20

			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter

			font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
			text: "Consulta para: " + searchH
		}

		ScrollView{

			id: scrollView
			clip: true

			anchors.bottom: parent.bottom
			anchors.right: parent.right
			anchors.left: parent.left
			anchors.top: parent.top

			anchors.topMargin: 2 * 20 + returnButton.height
			anchors.bottomMargin: 20
			anchors.rightMargin: 20
			anchors.leftMargin: 20

			contentChildren: Column{

				anchors.fill: parent
				spacing: 5

				Repeater{

					id: hotelLister
					model: hotelList

					Rectangle{

						width: scrollView.width	// Needed to avoid horizontal clipping
						color: "#00000000"
						height: 80

						Label{

							anchors.fill: parent

							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter

							font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
							color: "#ffffff"

							wrapMode: Text.WordWrap
							text: modelData
						}
					}
				}
			}
		}
	}

	Component.onCompleted: { toggleVisibility(); root.toggleVisibility(); }
	onClosing: root.toggleVisibility()
}