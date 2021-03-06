from PyQt5 import QtCore, QtGui, QtWidgets
import sys, os
import images
import datetime
import colorDlg
import pdfWriter
import book_generator
import session_manager
import sales_manager
import mysql.connector
    
def connect_db():
    try:
        f = open('database.txt', 'r')
        if f is None:
            pdfWriter.show_message('Warning', 'Can\'t open database.txt file')
            return None
        username = f.readline()
        username = username.strip('\r\n ')
        password = f.readline()
        password = password.strip('\r\n ')
    except:
        pdfWriter.show_message('Error', 'Exception occured')
        return None
    mydb = mysql.connector.connect(
        host="localhost",
        user=username,
        password=password,
        database="bingo"
        )
    if mydb is None:
        pdfWriter.show_message('warning', 'Can\'t connect to mysql')
        return None
    return mydb

mydb = connect_db()

class PicButton(QtWidgets.QAbstractButton):
    def __init__(self, pixmap, parent=None):
        super(PicButton, self).__init__(parent)
        self.pixmap = pixmap

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.drawPixmap(event.rect(), self.pixmap)

    def sizeHint(self):
        return self.pixmap.size()

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent=parent)
        
        if mydb is None:
            return
        self.tab_button_height = 45
        self.setTabs(self)
        self.sessionStack = session_manager.SessionManager()
        self.bookStack = book_generator.BookGenerator()
        self.salesStack = sales_manager.SalesManager()
        # self.setupUi(self.sessionStack, 0)
        # self
        # self.bookStack.setupUi()
        # self.setupUi(self.bookStack, 1)
        # self.setupUi(self.salesStack, 2)
        
        self.Stack = QtWidgets.QStackedWidget(self)
        self.Stack.setFixedSize(1000, 900)
        self.Stack.setGeometry(0, 20+self.tab_button_height, 1000, 900)
        self.Stack.addWidget(self.sessionStack)
        self.Stack.addWidget(self.bookStack)
        self.Stack.addWidget(self.salesStack)
        
        self.showBookGenerator()
        # self.showSessionManager()
        self.Stack.show()
        self.show()

    def setTabs(self, MainWindow):
        MainWindow.setFixedSize(1000, 900)
        MainWindow.resize(1000, 900)

        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.tab_button_layout_widget = QtWidgets.QWidget(self.centralwidget)
        self.tab_button_layout_widget.setGeometry(QtCore.QRect(80, 20, 770, self.tab_button_height))
        self.tab_button_layout_widget.setObjectName("tab_button_layout_widget")
        
        self.session_btn = PicButton(QtGui.QPixmap('./images/session_button.png'), self.tab_button_layout_widget)
        self.session_btn.clicked.connect(self.showSessionManager)

        self.book_btn = PicButton(QtGui.QPixmap('./images/generator_button.png'), self.tab_button_layout_widget)
        self.book_btn.clicked.connect(self.showBookGenerator)

        self.sales_btn = PicButton(QtGui.QPixmap('./images/sales_button.png'), self.tab_button_layout_widget)
        self.sales_btn.clicked.connect(self.showSalesManager)
        
        self.htab_button_box = QtWidgets.QHBoxLayout(self.tab_button_layout_widget)
        self.htab_button_box.setContentsMargins(0, 0, 0, 0)
        self.htab_button_box.setObjectName("htab_button_box")
        self.htab_button_box.addWidget(self.session_btn)
        self.htab_button_box.addWidget(self.book_btn)
        self.htab_button_box.addWidget(self.sales_btn)
        
        MainWindow.setCentralWidget(self.centralwidget)
        
    def showSessionManager(self):
        self.setStyleSheet("background-color: rgb(0, 104, 55);")
        self.setWindowTitle("Bingo Session Manager")
        self.Stack.setCurrentIndex(0)
        
    def showBookGenerator(self):
        self.setStyleSheet("background-color: #0067b1;")
        self.setWindowTitle("Bingo Ticket Generator")
        self.bookStack.loadSessions()
        self.Stack.setCurrentIndex(1)
        
    def showSalesManager(self):
        self.setStyleSheet("background-color: rgb(155, 97, 35);")
        self.setWindowTitle("Bingo Sales Manager")
        self.salesStack.loadSessions()
        self.Stack.setCurrentIndex(2)

    def setupUi(self, MainWindow, kind):
        MainWindow.setFixedSize(930, 900)
        MainWindow.resize(930, 900)
        if kind==0:
            MainWindow.setStyleSheet("background-color: rgb(0, 104, 55);")
            layout = QtWidgets.QFormLayout()
            sex = QtWidgets.QHBoxLayout()
            sex.addWidget(QtWidgets.QRadioButton("Male"))
            sex.addWidget(QtWidgets.QRadioButton("Female"))
            layout.addRow(QtWidgets.QLabel("Sex"),sex)
            layout.addRow("Date of Birth",QtWidgets.QLineEdit())
            MainWindow.setLayout(layout)

            MainWindow.setWindowTitle("Bingo Session Manager")
        if kind==1:
            MainWindow.setStyleSheet("background-color: #0067b1;")
            MainWindow.setWindowTitle("Bingo Ticket Generator")
            self.writeFirstLine(self.bookStack)
            self.writeSecondLine(self.bookStack)
            self.writeThirdLine(self.bookStack)
            self.writeFourthLine(self.bookStack)
            self.writeFifthLine(self.bookStack)
            self.writeSixthLine(self.bookStack)
            self.writeSeventhLine(self.bookStack)
            self.writeEighthLine(self.bookStack)
            self.drawButtons()
            self.setDefaultColors()
        if kind==2:
            MainWindow.setStyleSheet("background-color: rgb(155, 97, 35);")
            MainWindow.setWindowTitle("Bingo Sales Manager")
            
        self.show()
        



if __name__ == "__main__":
    ui_app = QtWidgets.QApplication(sys.argv)
    ui = MainWindow()
    ui.setWindowIcon(QtGui.QIcon("images/icon.png"))
    os._exit(ui_app.exec_())
