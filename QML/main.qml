import QtQuick.Controls 2.15	// For buttons and UI elements
import QtQuick.Layouts 1.15		// To use all layouts
import QtQuick.Dialogs 1.3		// To use Message and FileSystem Dialogs
import QtQuick.Window 2.15		// Tu use "Window" node
import QtQuick 2.15
import My.Lib 1.0

Window{

	id: window
	visible: false
	title: qsTr("Lista Hoteis - Tela 1")
	color: "#00000000"

	minimumHeight: 480/2
	minimumWidth: 640/2
	height: 480 * 0.75
	width: 640

	function toggleVisibility(){

		if( !window.visible ){

			setY(Screen.height/2 - height/2)
			setX(Screen.width/ 2 - width/2)
		}

		window.visible = !window.visible
	}

	Component.onCompleted: toggleVisibility()

	// Logic properties
	property alias hourText: hourInput.text
	property alias csvName: fileFound.text

	property InputProcessor controller: InputProcessor{}

	// Expansion/Contraction behaviors
	property alias lowerPartContraction: lowerPartContraction
	property alias lowerPartExpansion: lowerPartExpansion

	SequentialAnimation on height{

		// Animates thi window expanding
		id: windowExpansion
		running: false

		NumberAnimation{

			from: window.height
			to: window.height * 100/75
			duration: 1000
		}
	}

	SequentialAnimation on height{

		// Animates this window contraction
		id: windowContraction
		running: false

		NumberAnimation{

			from: window.height
			to: window.height * 75/100
			duration: 1000
		}
	}

	ColumnLayout{

		id: vbox_div
		anchors.fill: parent
		spacing: 0

// ################################
// ########### Top Part ###########
// ################################

		Rectangle{

			id: upperPart
			color: "#212222"
			
			Layout.preferredHeight: window.minimumHeight * 65/100
			Layout.fillHeight: true
			Layout.fillWidth: true

			RowLayout{

				id: upperInputReg
				spacing: 10

				height: parent.height * 40/100
				width: parent.width * 60/100
				anchors.centerIn: parent

				Rectangle{
					
					color: "#404142"
					border.width: 0.4
					radius: 5

					Layout.preferredWidth: parent.width * 60/100
					Layout.fillHeight: true
					Layout.fillWidth: true

					TextInput{

						id: hourInput
						anchors.fill: parent
						color: "#ffffff"

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter

						font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
						maximumLength: 5

						inputMask: "99:99"
						text: "00:00"
					}
				}

				Rectangle{

					readonly property Component newWindowComp: Qt.createComponent("HourListWindow.qml")

					id: hourButton
					color: "#00827F"
					radius: 5

					Layout.preferredWidth: parent.width * 40/100
					Layout.fillHeight: true
					Layout.fillWidth: true

					Label{

						anchors.fill: parent

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter

						font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
						color: "#000000"
						text: "Buscar"
					}

					function onClicked(){

						try{

							// Create the QStringList with C++
							var hList = controller.availableHours(csvName.toString(),hourText.toString())

							if( newWindowComp.status === Component.Ready ){

								var newWindow = newWindowComp.createObject(window,{

									root: window,
									searchH: hourText,
									hotelList: hList
								})

								if( newWindow === null )
									messageDialog.show("Internal error","Error creating object")

							}else if( newWindowComp.status === Component.Error ){

								messageDialog.show("Internal error","Error loading component:" + newWindowComp.errorString())
							}

						}catch( error){

							messageDialog.show("Input Error",error)
						}
					}

					// Turns it into a button
					MouseArea{
		
						onClicked: parent.onClicked()
						anchors.fill: parent

						onPressed: parent.color = "#006E6E"
						onReleased: parent.color = "#00827F"
					}
				}
			}
		}

// ################################
// ############ Middle ############
// ################################

		// This is a kind of button, wich controlls the next rectangle expansion/contraction
		Rectangle{

			id: advancedButton
			color: "#005457"

			Layout.preferredHeight: window.minimumHeight * 10/100
			Layout.fillHeight: true
			Layout.fillWidth: true
			
			Label{

				anchors.fill: parent

				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter

				font.pointSize: Math.max(Math.min(height,width) * 25/100,1)
				text: "Opções avançadas"
				font.italic: true
				color: "#ffffff"
			}

			function onClicked(){

				if( !lowerPartExpansion.running && !lowerPartContraction.running ){

					if( lowerPart.visible == true ){

						lowerPartContraction.running = true
						windowContraction.running = true

					}else{

						lowerPartExpansion.running = true
						windowExpansion.running = true
					}
				}
			}

			// Turns it into a button
			MouseArea{

				onClicked: parent.onClicked()
				anchors.fill: parent

				onPressed: parent.color = "#003336"
				onReleased: parent.color = "#005457"
			}
		}

// ################################
// ########## Lower Part ##########
// ################################

		Rectangle{

			id: lowerPart
			color: "#212222"

			// Changes with input
			visible: false

			Layout.preferredHeight: window.minimumHeight * 25/100
			Layout.fillHeight: true
			Layout.fillWidth: true

			RowLayout{

				id: lowerInputReg
				spacing: 10

				height: parent.height * 50/100
				width: parent.width * 80/100
				anchors.centerIn: parent

				Rectangle{

					color: "#404142"
					border.width: 0.4
					radius: 5

					Layout.preferredWidth: parent.width * 80/100
					Layout.fillHeight: true
					Layout.fillWidth: true

					Label{

						id: fileFound
						anchors.fill: parent

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter

						font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
						color: "#ffffff"

						text: "file://" + controller.getHomePath() + "/hotelList.csv"
						wrapMode: Text.WrapAnywhere
					}
				}

				Rectangle{

					id: fileButton
					color: "#00827F"
					radius: 5

					Layout.preferredWidth: parent.width * 20/100
					Layout.fillHeight: true
					Layout.fillWidth: true

					Label{

						anchors.fill: parent

						horizontalAlignment: Text.AlignHCenter
						verticalAlignment: Text.AlignVCenter

						font.pointSize: Math.max(Math.min(height,width) * 20/100,1)
						color: "#000000"
						text: "Buscar"
					}

					function onClicked(){ fileDialog.visible = true; }

					// Turns it into a button
					MouseArea{
		
						onClicked: parent.onClicked()
						anchors.fill: parent
		
						onPressed: parent.color = "#006E6E"
						onReleased: parent.color = "#00827F"
					}
				}
			}

			SequentialAnimation on Layout.preferredHeight{

				// Animates thi window lower part expanding
				id: lowerPartExpansion
				running: false

				NumberAnimation{
					
					from: 0
					to: window.minimumHeight * 25/100
					duration: 1000
				}

				onRunningChanged: { if( running == true ) lowerPart.visible = true; }
			}

			SequentialAnimation on Layout.preferredHeight{

				// Animates this window lower part contraction
				id: lowerPartContraction
				running: false

				NumberAnimation{
		
					from: window.minimumHeight * 25/100
					to: 0
					duration: 1000
				}

				onRunningChanged: { if( running == false ) lowerPart.visible = false; }
			}
		}
	}

	FileDialog{

		id: fileDialog
		title: "Please choose a .csv"
		folder: shortcuts.home

		nameFilters: ["CSV files (*.csv)"]

		selectMultiple: false
		selectExisting: true
		selectFolder: false

		onAccepted: fileFound.text = fileDialog.fileUrl
	}

	MessageDialog{

		id: messageDialog
		visible: false

		icon: StandardIcon.Warning
		title: "Title"
		text: "Text"

		function show( _title, _text, _icon = StandardIcon.Warning){

			title = _title.toString()
			text = _text.toString()
			icon = _icon

			visible = true
		}
	}
}
