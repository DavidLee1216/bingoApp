# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'csv_maker.ui'
#
# Created by: PyQt5 UI code generator 5.13.1
#
# WARNING! All changes made in this file will be lost!


import datetime
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog, QMessageBox
import writedata
import re

class Ui_CSVMAKER(object):
    def setupUi(self, CSVMAKER):
        CSVMAKER.setObjectName("CSVMAKER")
        CSVMAKER.resize(459, 235)
        self.groupBox = QtWidgets.QGroupBox(CSVMAKER)
        self.groupBox.setGeometry(QtCore.QRect(30, 20, 381, 191))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.groupBox.setFont(font)
        self.groupBox.setObjectName("groupBox")
        self.label = QtWidgets.QLabel(self.groupBox)
        self.label.setGeometry(QtCore.QRect(30, 30, 47, 13))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.label.setFont(font)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(self.groupBox)
        self.label_2.setGeometry(QtCore.QRect(30, 70, 47, 13))
        self.label_2.setAlignment(QtCore.Qt.AlignCenter)
        self.label_2.setObjectName("label_2")
        self.to_edit = QtWidgets.QLineEdit(self.groupBox)
        self.to_edit.setEnabled(True)
        self.to_edit.setGeometry(QtCore.QRect(90, 62, 133, 25))
        self.to_edit.setObjectName("to_edit")
        self.label_3 = QtWidgets.QLabel(self.groupBox)
        self.label_3.setGeometry(QtCore.QRect(16, 100, 71, 20))
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName("label_3")
        self.file_edit = QtWidgets.QLineEdit(self.groupBox)
        self.file_edit.setEnabled(True)
        self.file_edit.setGeometry(QtCore.QRect(90, 100, 221, 25))
        self.file_edit.setObjectName("file_edit")
        self.from_edit = QtWidgets.QLineEdit(self.groupBox)
        self.from_edit.setEnabled(True)
        self.from_edit.setGeometry(QtCore.QRect(90, 25, 133, 25))
        self.from_edit.setObjectName("from_edit")
        self.OpenCSVButton = QtWidgets.QPushButton(self.groupBox)
        self.OpenCSVButton.setGeometry(QtCore.QRect(320, 100, 41, 25))
        self.OpenCSVButton.setAutoDefault(False)
        self.OpenCSVButton.setObjectName("csvfileopen")
        self.OpenCSVButton.clicked.connect(self.open_csv_file)
    
        self.WriteToCSVButton = QtWidgets.QPushButton(self.groupBox)
        self.WriteToCSVButton.setGeometry(QtCore.QRect(140, 150, 75, 23))
        self.WriteToCSVButton.setObjectName("write_to_csv")
        self.WriteToCSVButton.clicked.connect(self.writeToCSV)

        self.retranslateUi(CSVMAKER)
        QtCore.QMetaObject.connectSlotsByName(CSVMAKER)

    def show_message(self, title, text):
        msg = QMessageBox()
        msg.setIcon(QMessageBox.Warning)
        msg.setText(text)
        msg.setWindowTitle(title)
        msg.setStandardButtons(QMessageBox.Ok)
        retval = msg.exec_()

    def retranslateUi(self, CSVMAKER):
        _translate = QtCore.QCoreApplication.translate
        CSVMAKER.setWindowTitle(_translate("CSVMAKER", "Dialog"))
        self.groupBox.setTitle(_translate("CSVMAKER", "Panel id scope"))
        self.label.setText(_translate("CSVMAKER", "From"))
        self.label_2.setText(_translate("CSVMAKER", "To"))
        self.label_3.setText(_translate("CSVMAKER", "Output File"))
        self.OpenCSVButton.setText(_translate("CSVMAKER", "..."))
        self.WriteToCSVButton.setText(_translate("CSVMAKER", "To CSV"))

    def open_csv_file(self):
        filename = self.openFileNameDialog()
        self.file_edit.setText(filename)
        return

    def openFileNameDialog(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getSaveFileName(None,"Choose File", "","All Files (*);;csv Files (*.csv)", options=options)
#        if fileName:
#            print(fileName)
        return fileName

    def writeToCSV(self):
        filename = self.file_edit.text()
        from_id = self.from_edit.text()
        to_id = self.to_edit.text()
        try:
            f = open('database.txt', 'r')
            if f is None:
                self.show_message('Warning', 'Can\'t open database.txt file')
                return
            username = f.readline()
            username = username.strip('\r\n ')
            password = f.readline()
            password = password.strip('\r\n ')
            if from_id.isdigit()==False or to_id.isdigit()==False:
                self.show_message('Warning', 'Select number!')
                return
            nFromId = int(from_id)
            nToId = int(to_id)
            if nToId < nFromId:
                self.show_message('Warning', 'number \'to\' must be grater than \'from\'')
                return
            if nFromId==0 or nToId==0:
                self.show_message('Warning', 'The number can\'t be 0')
                return
            if filename=='':
                self.show_message('Warning', 'Please choose csv file')
                return
            res = writedata.writecsv(nFromId, nToId, username, password, filename)
            if res==-1:
                self.show_message('Warning', 'Can\'t open csv file')
            elif res==-2:
                self.show_message('Warning', 'Can\'t open database')
            else:
                self.show_message('Success', 'Successfully completed to write csv')
        except Exception as e:
            self.show_message('Warning', 'Exception occured')

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    CSVMAKER = QtWidgets.QDialog()
    ui = Ui_CSVMAKER()
    ui.setupUi(CSVMAKER)
    CSVMAKER.show()
    sys.exit(app.exec_())
