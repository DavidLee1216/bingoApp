# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'colorDlg.ui'
#
# Created by: PyQt5 UI code generator 5.13.1
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(400, 500)
        self.buttonBox = QtWidgets.QDialogButtonBox(Dialog)
        self.buttonBox.setGeometry(QtCore.QRect(-70, 440, 351, 32))
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")

        self.gold = QtWidgets.QRadioButton(Dialog)
        self.gold.setGeometry(QtCore.QRect(50, 20, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.gold.setFont(font)
        self.gold.setStyleSheet("background-color: gold;")
        self.gold.setObjectName("gold")
        self.gold.clicked.connect(self.goldClicked)
        self.white = QtWidgets.QRadioButton(Dialog)
        self.white.setGeometry(QtCore.QRect(160, 20, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.white.setFont(font)
        self.white.setStyleSheet("background-color: white;")
        self.white.setObjectName("white")
        self.white.clicked.connect(self.whiteClicked)
        self.silver = QtWidgets.QRadioButton(Dialog)
        self.silver.setGeometry(QtCore.QRect(270, 20, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.silver.setFont(font)
        self.silver.setStyleSheet("background-color: silver;")
        self.silver.setObjectName("silver")
        self.silver.clicked.connect(self.silverClicked)

        self.red = QtWidgets.QRadioButton(Dialog)
        self.red.setGeometry(QtCore.QRect(50, 60, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.red.setFont(font)
        self.red.setStyleSheet("background-color: red;")
        self.red.setObjectName("red")
        self.red.clicked.connect(self.redClicked)
        self.light_red = QtWidgets.QRadioButton(Dialog)
        self.light_red.setGeometry(QtCore.QRect(160, 60, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_red.setFont(font)
        self.light_red.setStyleSheet("background-color: rgb(255, 204, 203);")
        self.light_red.setObjectName("light_red")
        self.light_red.clicked.connect(self.lightredClicked)
        self.dark_red = QtWidgets.QRadioButton(Dialog)
        self.dark_red.setGeometry(QtCore.QRect(270, 60, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_red.setFont(font)
        self.dark_red.setStyleSheet("background-color: darkred; color:white")
        self.dark_red.setObjectName("dark_red")
        self.dark_red.clicked.connect(self.darkredClicked)

        self.green = QtWidgets.QRadioButton(Dialog)
        self.green.setGeometry(QtCore.QRect(50, 100, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.green.setFont(font)
        self.green.setStyleSheet("background-color: rgb(0, 255, 0);")
        self.green.setObjectName("green")
        self.green.clicked.connect(self.greenClicked)
        self.light_green = QtWidgets.QRadioButton(Dialog)
        self.light_green.setGeometry(QtCore.QRect(160, 100, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_green.setFont(font)
        self.light_green.setStyleSheet("background-color: lightgreen;")
        self.light_green.setObjectName("light_green")
        self.light_green.clicked.connect(self.lightgreenClicked)
        self.dark_green = QtWidgets.QRadioButton(Dialog)
        self.dark_green.setGeometry(QtCore.QRect(270, 100, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_green.setFont(font)
        self.dark_green.setStyleSheet("background-color: darkgreen; color:white")
        self.dark_green.setObjectName("dark_green")
        self.dark_green.clicked.connect(self.darkgreenClicked)

        self.blue = QtWidgets.QRadioButton(Dialog)
        self.blue.setGeometry(QtCore.QRect(50, 140, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.blue.setFont(font)
        self.blue.setStyleSheet("background-color: blue; color:white")
        self.blue.setObjectName("blue")
        self.blue.clicked.connect(self.blueClicked)
        self.light_blue = QtWidgets.QRadioButton(Dialog)
        self.light_blue.setGeometry(QtCore.QRect(160, 140, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_blue.setFont(font)
        self.light_blue.setStyleSheet("background-color: lightblue;")
        self.light_blue.setObjectName("light_blue")
        self.light_blue.clicked.connect(self.lightblueClicked)
        self.dark_blue = QtWidgets.QRadioButton(Dialog)
        self.dark_blue.setGeometry(QtCore.QRect(270, 140, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_blue.setFont(font)
        self.dark_blue.setStyleSheet("background-color: darkblue; color:white")
        self.dark_blue.setObjectName("dark_blue")
        self.dark_blue.clicked.connect(self.darkblueClicked)

        self.yellow = QtWidgets.QRadioButton(Dialog)
        self.yellow.setGeometry(QtCore.QRect(50, 180, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.yellow.setFont(font)
        self.yellow.setStyleSheet("background-color: yellow;")
        self.yellow.setObjectName("yellow")
        self.yellow.clicked.connect(self.yellowClicked)
        self.light_yellow = QtWidgets.QRadioButton(Dialog)
        self.light_yellow.setGeometry(QtCore.QRect(160, 180, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_yellow.setFont(font)
        self.light_yellow.setStyleSheet("background-color: lightyellow;")
        self.light_yellow.setObjectName("light_yellow")
        self.light_yellow.clicked.connect(self.lightyellowClicked)
        self.dark_yellow = QtWidgets.QRadioButton(Dialog)
        self.dark_yellow.setGeometry(QtCore.QRect(270, 180, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_yellow.setFont(font)
        self.dark_yellow.setStyleSheet("background-color: rgb(102,102,0); color:white")
        self.dark_yellow.setObjectName("dark_yellow")
        self.dark_yellow.clicked.connect(self.darkyellowClicked)

        self.orange = QtWidgets.QRadioButton(Dialog)
        self.orange.setGeometry(QtCore.QRect(50, 220, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.orange.setFont(font)
        self.orange.setStyleSheet("background-color: orange;")
        self.orange.setObjectName("orange")
        self.orange.clicked.connect(self.orangeClicked)
        self.light_orange = QtWidgets.QRadioButton(Dialog)
        self.light_orange.setGeometry(QtCore.QRect(160, 220, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_orange.setFont(font)
        self.light_orange.setStyleSheet("background-color: rgb(255, 207, 158);")
        self.light_orange.setObjectName("light_orange")
        self.light_orange.clicked.connect(self.lightorangeClicked)
        self.dark_orange = QtWidgets.QRadioButton(Dialog)
        self.dark_orange.setGeometry(QtCore.QRect(270, 220, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_orange.setFont(font)
        self.dark_orange.setStyleSheet("background-color: darkorange;")
        self.dark_orange.setObjectName("dark_orange")
        self.dark_orange.clicked.connect(self.darkorangeClicked)

        self.purple = QtWidgets.QRadioButton(Dialog)
        self.purple.setGeometry(QtCore.QRect(50, 260, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.purple.setFont(font)
        self.purple.setStyleSheet("background-color: purple; color:white")
        self.purple.setObjectName("purple")
        self.purple.clicked.connect(self.purpleClicked)
        self.light_purple = QtWidgets.QRadioButton(Dialog)
        self.light_purple.setGeometry(QtCore.QRect(160, 260, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_purple.setFont(font)
        self.light_purple.setStyleSheet("background-color: rgb(160, 32, 240); color:white")
        self.light_purple.setObjectName("light_purple")
        self.light_purple.clicked.connect(self.lightpurpleClicked)
        self.dark_purple = QtWidgets.QRadioButton(Dialog)
        self.dark_purple.setGeometry(QtCore.QRect(270, 260, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_purple.setFont(font)
        self.dark_purple.setStyleSheet("background-color: rgb(128,0,191); color:white")
        self.dark_purple.setObjectName("dark_purple")
        self.dark_purple.clicked.connect(self.darkpurpleClicked)
        
        self.grey = QtWidgets.QRadioButton(Dialog)
        self.grey.setGeometry(QtCore.QRect(50, 300, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.grey.setFont(font)
        self.grey.setStyleSheet("background-color: grey;")
        self.grey.setObjectName("grey")
        self.grey.clicked.connect(self.greyClicked)
        self.light_grey = QtWidgets.QRadioButton(Dialog)
        self.light_grey.setGeometry(QtCore.QRect(160, 300, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_grey.setFont(font)
        self.light_grey.setStyleSheet("background-color: lightgrey;")
        self.light_grey.setObjectName("light_grey")
        self.light_grey.clicked.connect(self.lightgreyClicked)
        self.dark_grey = QtWidgets.QRadioButton(Dialog)
        self.dark_grey.setGeometry(QtCore.QRect(270, 300, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_grey.setFont(font)
        self.dark_grey.setStyleSheet("background-color: darkgrey;")
        self.dark_grey.setObjectName("dark_grey")
        self.dark_grey.clicked.connect(self.darkgreyClicked)

        self.brown = QtWidgets.QRadioButton(Dialog)
        self.brown.setGeometry(QtCore.QRect(50, 340, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.brown.setFont(font)
        self.brown.setStyleSheet("background-color: brown; color:white")
        self.brown.setObjectName("brown")
        self.brown.clicked.connect(self.brownClicked)
        self.light_brown = QtWidgets.QRadioButton(Dialog)
        self.light_brown.setGeometry(QtCore.QRect(160, 340, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_brown.setFont(font)
        self.light_brown.setStyleSheet("background-color: rgb(181, 101, 29); color:white")
        self.light_brown.setObjectName("light_brown")
        self.light_brown.clicked.connect(self.lightbrownClicked)
        self.dark_brown = QtWidgets.QRadioButton(Dialog)
        self.dark_brown.setGeometry(QtCore.QRect(270, 340, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_brown.setFont(font)
        self.dark_brown.setStyleSheet("background-color: rgb(101,67,33); color:white")
        self.dark_brown.setObjectName("dark_brown")
        self.dark_brown.clicked.connect(self.darkbrownClicked)

        self.pink = QtWidgets.QRadioButton(Dialog)
        self.pink.setGeometry(QtCore.QRect(50, 380, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.pink.setFont(font)
        self.pink.setStyleSheet("background-color: pink;")
        self.pink.setObjectName("pink")
        self.pink.clicked.connect(self.pinkClicked)
        self.light_pink = QtWidgets.QRadioButton(Dialog)
        self.light_pink.setGeometry(QtCore.QRect(160, 380, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.light_pink.setFont(font)
        self.light_pink.setStyleSheet("background-color: lightpink;")
        self.light_pink.setObjectName("light_pink")
        self.light_pink.clicked.connect(self.lightpinkClicked)
        self.dark_pink = QtWidgets.QRadioButton(Dialog)
        self.dark_pink.setGeometry(QtCore.QRect(270, 380, 90, 31))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.dark_pink.setFont(font)
        self.dark_pink.setStyleSheet("background-color: rgb(231, 84, 128); color:white")
        self.dark_pink.setObjectName("dark_pink")
        self.dark_pink.clicked.connect(self.darkpinkClicked)
        # self.gold = QtWidgets.QRadioButton(Dialog)
        # self.gold.setGeometry(QtCore.QRect(50, 20, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.gold.setFont(font)
        # self.gold.setStyleSheet("background-color: gold;")
        # self.gold.setObjectName("gold")
        # self.gold.clicked.connect(self.goldClicked)

        # self.purple = QtWidgets.QRadioButton(Dialog)
        # self.purple.setGeometry(QtCore.QRect(50, 60, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.purple.setFont(font)
        # self.purple.setStyleSheet("background-color: purple;")
        # self.purple.setObjectName("purple")
        # self.purple.clicked.connect(self.purpleClicked)

        # self.blue = QtWidgets.QRadioButton(Dialog)
        # self.blue.setGeometry(QtCore.QRect(50, 100, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.blue.setFont(font)
        # self.blue.setStyleSheet("background-color: blue;")
        # self.blue.setObjectName("blue")
        # self.blue.clicked.connect(self.blueClicked)

        # self.green = QtWidgets.QRadioButton(Dialog)
        # self.green.setGeometry(QtCore.QRect(50, 140, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.green.setFont(font)
        # self.green.setStyleSheet("background-color: green;")
        # self.green.setObjectName("green")
        # self.green.clicked.connect(self.greenClicked)

        # self.pink = QtWidgets.QRadioButton(Dialog)
        # self.pink.setGeometry(QtCore.QRect(50, 180, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.pink.setFont(font)
        # self.pink.setStyleSheet("background-color: pink;")
        # self.pink.setObjectName("pink")
        # self.pink.clicked.connect(self.pinkClicked)

        # self.yellow = QtWidgets.QRadioButton(Dialog)
        # self.yellow.setGeometry(QtCore.QRect(160, 20, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.yellow.setFont(font)
        # self.yellow.setStyleSheet("background-color: yellow;")
        # self.yellow.setObjectName("yellow")
        # self.yellow.clicked.connect(self.yellowClicked)

        # self.red = QtWidgets.QRadioButton(Dialog)
        # self.red.setGeometry(QtCore.QRect(160, 60, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.red.setFont(font)
        # self.red.setStyleSheet("background-color: red;")
        # self.red.setObjectName("red")
        # self.red.clicked.connect(self.redClicked)

        # self.lilac = QtWidgets.QRadioButton(Dialog)
        # self.lilac.setGeometry(QtCore.QRect(160, 100, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.lilac.setFont(font)
        # self.lilac.setStyleSheet("background-color: rgb(200, 162, 200);")
        # self.lilac.setObjectName("lilac")
        # self.lilac.clicked.connect(self.lilacClicked)

        # self.brown = QtWidgets.QRadioButton(Dialog)
        # self.brown.setGeometry(QtCore.QRect(160, 140, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.brown.setFont(font)
        # self.brown.setStyleSheet("background-color: brown;")
        # self.brown.setObjectName("brown")
        # self.brown.clicked.connect(self.brownClicked)

        # self.orange = QtWidgets.QRadioButton(Dialog)
        # self.orange.setGeometry(QtCore.QRect(160, 180, 82, 31))
        # font = QtGui.QFont()
        # font.setPointSize(10)
        # self.orange.setFont(font)
        # self.orange.setStyleSheet("background-color: orange;")
        # self.orange.setObjectName("orange")
        # self.orange.clicked.connect(self.orangeClicked)

        self.retranslateUi(Dialog)
        self.buttonBox.accepted.connect(Dialog.accept)
        self.buttonBox.rejected.connect(Dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Select colour"))
        self.gold.setText(_translate("Dialog", "gold"))
        self.white.setText(_translate("Dialog", "white"))
        self.silver.setText(_translate("Dialog", "silver"))
        self.purple.setText(_translate("Dialog", "purple"))
        self.light_purple.setText(_translate("Dialog", "light purple"))
        self.dark_purple.setText(_translate("Dialog", "dark purple"))
        self.yellow.setText(_translate("Dialog", "yellow"))
        self.light_yellow.setText(_translate("Dialog", "light yellow"))
        self.dark_yellow.setText(_translate("Dialog", "dark yellow"))
        self.pink.setText(_translate("Dialog", "pink"))
        self.light_pink.setText(_translate("Dialog", "light pink"))
        self.dark_pink.setText(_translate("Dialog", "dark pink"))
        self.green.setText(_translate("Dialog", "green"))
        self.light_green.setText(_translate("Dialog", "light green"))
        self.dark_green.setText(_translate("Dialog", "dark green"))
        self.orange.setText(_translate("Dialog", "orange"))
        self.light_orange.setText(_translate("Dialog", "light orange"))
        self.dark_orange.setText(_translate("Dialog", "dark orange"))
        self.blue.setText(_translate("Dialog", "blue"))
        self.light_blue.setText(_translate("Dialog", "light blue"))
        self.dark_blue.setText(_translate("Dialog", "dark blue"))
        self.red.setText(_translate("Dialog", "red"))
        self.light_red.setText(_translate("Dialog", "light red"))
        self.dark_red.setText(_translate("Dialog", "dark red"))
        self.brown.setText(_translate("Dialog", "brown"))
        self.light_brown.setText(_translate("Dialog", "light brown"))
        self.dark_brown.setText(_translate("Dialog", "dark brown"))
        self.grey.setText(_translate("Dialog", "grey"))
        self.light_grey.setText(_translate("Dialog", "light grey"))
        self.dark_grey.setText(_translate("Dialog", "dark grey"))

    def redClicked(self):
        self.curr_color = "red"
        self.color_name = "red"
    
    def blueClicked(self):
        self.curr_color = "blue"
        self.color_name = "blue"

    def greenClicked(self):
        self.curr_color = "rgb(0, 255, 0)"
        self.color_name = "green"

    def brownClicked(self):
        self.curr_color = "brown"
        self.color_name = "brown"

    def orangeClicked(self):
        self.curr_color = "orange"
        self.color_name = "orange"

    def yellowClicked(self):
        self.curr_color = "yellow"
        self.color_name = "yellow"

    def goldClicked(self):
        self.curr_color = "gold"
        self.color_name = "gold"

    def pinkClicked(self):
        self.curr_color = "pink"
        self.color_name = "pink"

    def purpleClicked(self):
        self.curr_color = "purple"
        self.color_name = "purple"

    def whiteClicked(self):
        self.curr_color = "white"
        self.color_name = "white"

    def silverClicked(self):
        self.curr_color = "silver"
        self.color_name = "silver"

    def lightredClicked(self):
        self.curr_color = "rgb(255, 204, 203)"
        self.color_name = "lightred"

    def darkredClicked(self):
        self.curr_color = "darkred"
        self.color_name = "darkred"

    def lightgreenClicked(self):
        self.curr_color = "lightgreen"
        self.color_name = "lightgreen"

    def darkgreenClicked(self):
        self.curr_color = "darkgreen"
        self.color_name = "darkgreen"

    def lightblueClicked(self):
        self.curr_color = "lightblue"
        self.color_name = "lightblue"

    def darkblueClicked(self):
        self.curr_color = "darkblue"
        self.color_name = "darkblue"

    def lightyellowClicked(self):
        self.curr_color = "lightyellow"
        self.color_name = "lightyellow"

    def darkyellowClicked(self):
        self.curr_color = "rgb(102, 102, 0)"
        self.color_name = "darkyellow"

    def lightorangeClicked(self):
        self.curr_color = "rgb(255, 165, 0)"
        self.color_name = "lightorange"

    def darkorangeClicked(self):
        self.curr_color = "darkorange"
        self.color_name = "darkorange"

    def lightpurpleClicked(self):
        self.curr_color = "rgb(160, 32, 240)"
        self.color_name = "lightpurple"

    def darkpurpleClicked(self):
        self.curr_color = "rgb(128,0,191)"
        self.color_name = "darkpurple"

    def greyClicked(self):
        self.curr_color = "grey"
        self.color_name = "grey"

    def lightgreyClicked(self):
        self.curr_color = "lightgrey"
        self.color_name = "lightgrey"

    def darkgreyClicked(self):
        self.curr_color = "darkgrey"
        self.color_name = "darkgrey"

    def lightbrownClicked(self):
        self.curr_color = "rgb(181, 101, 29)"
        self.color_name = "lightbrown"

    def darkbrownClicked(self):
        self.curr_color = "rgb(101,67,33)"
        self.color_name = "darkbrown"

    def lightpinkClicked(self):
        self.curr_color = "lightpink"
        self.color_name = "lightpink"

    def darkpinkClicked(self):
        self.curr_color = "rgb(231, 84, 128)"
        self.color_name = "darkpink"
 
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Dialog = QtWidgets.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())
