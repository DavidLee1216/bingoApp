from fpdf import FPDF
import mysql.connector
from PyQt5 import QtCore, QtGui, QtWidgets
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4, landscape
from reportlab.lib.units import mm
from PIL import Image
from reportlab.pdfbase.pdfmetrics import stringWidth
import io
from PyPDF2 import PdfFileReader, PdfFileWriter, PdfFileMerger
from pdfrw import PdfReader, PdfWriter, PageMerge
import gui

pdf = FPDF('L', 'mm', 'A4')

colors = {  'red':(255, 0, 0),
            'green':(0, 255, 0),
            'blue':(0, 0, 255),
            'gold':(255, 215, 0),
            'brown':(165,42,42),
            'orange':(255,165,0),
            'yellow':(255,255,0),
            'grey':(128, 128, 128),
            'pink':(255,192,203),
            'purple':(128,0,128),
            'white':(255, 255, 255),
            'silver':(192, 192, 192),
            'lightred':(255, 204, 203),
            'darkred':(139, 0, 0),
            'lightgreen':(144, 238, 144),
            'darkgreen':(0, 100, 0),
            'lightblue':(173, 216, 230),
            'darkblue':(0, 0, 139),
            'lightbrown':(181, 101, 29),
            'darkbrown':(101,67,33),
            'lightorange':(255, 207, 158),
            'darkorange':(255, 140, 0),
            'lightyellow':(255, 255, 224),
            'darkyellow':(102,102,0),
            'lightgrey':(211, 211, 211),
            'darkgrey':(169, 169, 169),
            'lightpink':(255, 182, 193),
            'darkpink':(231, 84, 128),
            'lightpurple':(160, 32, 240),
            'darkpurple':(128,0,191)
}

book_kind = ['single', 'double', 'sheet']
page_width = 297
page_height = 210
top_margin = 10
left_margin = 8
right_margin = 8
logo_height = 15
page_title_height = 8
panel_interval = 7
page_title_interval = 3
panel_vertical_interval = 3
panel_number_height = 5

def show_message(title, text, style='MB_OK'):
    msg = QtWidgets.QMessageBox()
    msg.setIcon(QtWidgets.QMessageBox.Warning)
    msg.setText(text)
    msg.setWindowTitle(title)
    if style=='MB_YESNO':
        msg.setStandardButtons(QtWidgets.QMessageBox.Yes|QtWidgets.QMessageBox.No)
        msg.setDefaultButton(QtWidgets.QMessageBox.Yes)
        buttonYes = msg.button(QtWidgets.QMessageBox.Yes)
        buttonNo = msg.button(QtWidgets.QMessageBox.No)
        msg.exec_()
        if msg.clickedButton()==buttonYes:
            return 1
        else:
            return 0
    elif style=='MB_OK':
        msg.setStandardButtons(QtWidgets.QMessageBox.Ok)
    retval = msg.exec_()
    return retval

def makePdf_using_fpdf(bingo_pages_per_book, bingo_books_to_print, perm_range_from, perm_range_to, page_title, logo_path, date_from, date_to, panel_colors):
    mydb = mysql.connector.connect(
    host="localhost",
    user="root",
    password="Quoted1216",
    database="bingo"
    )

    mycursor = mydb.cursor()
    try:
        pdf.set_auto_page_break(False)
        start_table_id = perm_range_from%6
        if start_table_id==0:
            start_table_id = 6
        end_table_id = perm_range_to%6
        if end_table_id==0:
            end_table_id = 6
        start_panel = perm_range_from-start_table_id+1
        end_panel = perm_range_to+6-end_table_id
        color_idx = 0
        while color_idx < len(panel_colors):
            if panel_colors[color_idx]==None:
                panel_colors.pop(color_idx)
            else:
                color_idx += 1
        curr_color_idx = 0
        panel_color_cnt = len(panel_colors)
        ret = show_message('Notification', 'The program will start to print from '+str(start_panel)+' to '+str(end_panel)+'\r\nAre you going to continue?', 'MB_YESNO')
        if ret==0:
            return
        if ret==1:
            logo_width = page_width/3-left_margin-right_margin
            card_idx = 0
            for perm in range(start_panel, end_panel+1):
                nTableId = perm % 6
                if nTableId == 0:
                    nTableId = 6
                if nTableId != 1:
                    continue
                rgb = colors[panel_colors[curr_color_idx]]
                pdf.set_font('Arial', '', 16)
                if card_idx%3 == 0:
                    pdf.add_page()    
                pdf.set_fill_color(rgb[0], rgb[1], rgb[2])
                curr_color_idx += 1
                if curr_color_idx==panel_color_cnt:
                    curr_color_idx = 0
                pdf.rect((card_idx%3)*(page_width/3), 0, page_width/3, page_height, 'F')
                pdf.set_fill_color(255, 255, 255)
                pdf.rect((card_idx%3)*(page_width/3)+left_margin, top_margin, logo_width, logo_height, 'F')
                pdf.image(logo_path, (card_idx%3)*(page_width/3)+left_margin, top_margin, logo_width, logo_height)
                pdf.rect((card_idx%3)*(page_width/3)+left_margin, top_margin+logo_height+panel_interval, logo_width, page_title_height, 'F')
                pdf.set_xy((card_idx%3)*(page_width/3)+left_margin, top_margin+logo_height+panel_interval)
                pdf.set_text_color(0, 0, 0)
                pdf.cell(logo_width, page_title_height, page_title, 0, 1, 'C')

                cell_width = (page_width/3-left_margin-panel_number_height-panel_interval-right_margin)/9
                cell_height = (page_height-top_margin*3-logo_height-page_title_height-panel_interval*7)/18

                pdf.set_xy((card_idx%3)*(page_width/3)+left_margin, top_margin+logo_height+panel_interval*2+page_title_height)
                pdf.cell(0, 0, '', 0, 1)
                # pdf.rect((card_idx%3)*(page_width/3)+left_margin+panel_number_height+panel_interval, top_margin+logo_height+panel_interval*2+page_title_height,
                #     (logo_width-panel_number_height-panel_interval), page_height-logo_height-page_title_height-panel_interval*2-top_margin*3, 'F')

                query = "select panel_1.panel_id, panel_1.numbers, panel_2.panel_id, panel_2.numbers, panel_3.panel_id, panel_3.numbers, panel_4.panel_id, panel_4.numbers, panel_5.panel_id, panel_5.numbers,\
                    panel_6.panel_id, panel_6.numbers from panel_1, panel_2, panel_3, panel_4, panel_5, panel_6 where panel_{:d}.panel_id='{:d}' and panel_{:d}.card_id=panel_1.card_id and panel_{:d}.card_id=panel_2.card_id \
                    and panel_{:d}.card_id=panel_3.card_id and panel_{:d}.card_id=panel_4.card_id and panel_{:d}.card_id=panel_5.card_id and panel_{:d}.card_id=panel_6.card_id".format(nTableId, perm, nTableId,
                    nTableId, nTableId, nTableId, nTableId, nTableId)
                mycursor.execute(query)
                myresult = mycursor.fetchall()
                if len(myresult) >= 1:
                    pdf.set_text_color(0, 0, 0) 
                    for i in range(0, 6):
                        panel_id = myresult[0][i*2]
                        panel_numbers_str = myresult[0][i*2+1]
                        panel_info = []
                        for j in range(0, 3):
                            panel_line = []
                            pdf.cell((card_idx%3)*(page_width/3)+panel_number_height+panel_interval)
                            for k in range(0, 9):
                                ch = str(int(panel_numbers_str[j*2*9+k*2:j*2*9+(k+1)*2]))
                                if ch=='0':
                                    ch = ''
                                panel_line.append(ch)
    #                            pdf.cell(80)
                                if k==8:
                                    pdf.cell(cell_width, cell_height, ch, 1, 1, 'C', True)
                                else:
                                    pdf.cell(cell_width, cell_height, ch, 1, 0, 'C', True)
                            panel_info.append(panel_line)
                        pdf.cell(0, panel_interval, '', 0, 1)
                    pdf.cell((card_idx%3)*(page_width/3)+1)
                    pdf.set_text_color(255, 255, 255) 
                    pdf.cell(logo_width, page_title_height, date_from+" - "+date_to, 0, 0, 'C')
                    
                card_idx += 1

            pdf.output('test.pdf', 'F')
            # doc = fitz.open('test.pdf')
            # page = doc[0]
            # point = fitz.Point(10, 50)
            # page.insertText(point, text='asdfasd', fontsize=8, fontname='Helvetica', color=(0, 0, 0), rotate=90)
            # doc.save('test.pdf', incremental=True)#
            # doc.close()
    except Exception as e:
        kk = 0
    mycursor.close()
    mydb.close()
    return

def get_real_y(y):
    return page_height*mm-y

def checkPermRangeExistenceTicket(mydb, mycursor, perm_from, perm_to, session_id):
    query = "select *from print_info where session_id='{}'".format(session_id)
    mycursor.execute(query)
    myresult = mycursor.fetchall()
    for result in myresult:
        perm_range_from = int(result[3])
        perm_range_to = int(result[4])
        if perm_to < perm_range_from or perm_from > perm_range_to:
            continue
        else:
            return True
    return False

def getLastId(mydb, mycursor, tablename):
    query = "select id from {} order by id desc".format(tablename)
    mycursor.execute(query)
    myresult = mycursor.fetchone()
    if len(myresult)==0:
        return 0
    else:
        return int(myresult[0])
    
def addToPrintInfo(mydb, mycursor, perm_from, perm_to, print_kind, session_id):
    query = "insert into print_info(id, session_id, print_kind, perm_from, perm_to, sold) values(NULL, '{}', '{}', '{}', '{}', '0')".format(session_id, print_kind, perm_from, perm_to)
    mycursor.execute(query)
    mydb.commit()
    return getLastId(mydb, mycursor, 'print_info')
        
    
def addTicketToDatabase(mydb, mycursor, card_id, panel_id, session_id, print_id, panel_color):
    query = "insert into soldtickets_new (id, card_id, panel_id, session_id, print_id, price, color, customer_name, sold) values(NULL, '{}', '{}', '{}', '{}', '0', '{}', '', '0')".format(card_id, panel_id, session_id, print_id, panel_color)
    mycursor.execute(query)
    mydb.commit()

def getDateString(mydb, mycursor, session_id):
    query = "select * from game_session_info where id='{}'".format(session_id)
    mycursor.execute(query)
    myresult = mycursor.fetchone()
    if len(myresult)==0:
        return ''
    else:
        date_from = str(myresult[2]) + " " + str(myresult[4])
        date_to = str(myresult[2]) + " " + str(myresult[5])
        return date_from + ' - ' + date_to
    
def makePdf(session_id, session_name, print_kind, bingo_pages_per_book, bingo_books_to_print, bingo_double_books_to_print, bingo_single_sheets_to_print, perm_range_from, perm_range_to, page_title, logo_path, panel_colors, single_sheet_color):
    mydb = gui.mydb
    print_double = True if print_kind == 1 else False
    print_sheet = True if print_kind == 2 else False
    if print_double==True:
        bingo_books_to_print = bingo_double_books_to_print*2
    if print_sheet==True:
        bingo_books_to_print = bingo_single_sheets_to_print
    # if print_double:
    #     print_kind = 1
    # elif print_sheet:
    #     print_kind = 2
    # else:
    #     print_kind = 0
    # print_kind = book_kind
    mycursor = mydb.cursor(buffered=True)
    try:
        if perm_range_from < 1:
            show_message("Warning", "Perm from range must be greater than 0")
            return
        start_table_id = perm_range_from%6
        if start_table_id==0:
            start_table_id = 6
        end_table_id = perm_range_to%6
        if end_table_id==0:
            end_table_id = 6
        start_panel = perm_range_from-start_table_id+1
        end_panel = perm_range_to+6-end_table_id
        if perm_range_to==0:
            end_panel = start_panel+6*bingo_pages_per_book*bingo_books_to_print-1
        color_idx = 0
        while color_idx < len(panel_colors):
            if panel_colors[color_idx]==None:
                panel_colors.pop(color_idx)
            else:
                color_idx += 1
        print_perm_count = bingo_pages_per_book*6 if print_kind!=1 else bingo_pages_per_book*6*2
        mycanvas = canvas.Canvas('books/{}-{}-({}-{}).pdf'.format(session_name, book_kind[print_kind], start_panel, print_perm_count+start_panel-1))
        
        mycanvas.setPageSize(landscape(A4))
        mycanvas.setFont('Helvetica', 16)
        curr_color_idx = 0
        panel_color_cnt = len(panel_colors)
        page_number = 0
        if panel_color_cnt==0:
            panel_colors.append("gold")
            panel_color_cnt = 1
        if print_sheet==True:
            panel_colors.clear()
            panel_colors.append(single_sheet_color)
            panel_color_cnt = 1
        ret = show_message('Notification', 'The program will start to print from '+str(start_panel)+' to '+str(end_panel)+'\r\nAre you going to continue?', 'MB_YESNO')
        if ret==0:
            return
        if ret==1:
            if checkPermRangeExistenceTicket(mydb, mycursor, start_panel, end_panel, session_id):
                show_message('Warning', 'Some perms are already printed in this session. Please choose another perm range.')
                return
            print_id = addToPrintInfo(mydb, mycursor, start_panel, print_perm_count+start_panel-1, print_kind, session_id)
            date_string = getDateString(mydb, mycursor, session_id)
            logo_width = page_width/3-left_margin-right_margin
            card_idx = 0
            new_canvas = False
            book_id = 1
            for perm in range(start_panel, end_panel+1):
                nTableId = perm % 6
                if nTableId == 0:
                    nTableId = 6
                if nTableId != 1:
                    continue
                if new_canvas:
                    print_perm_count = bingo_pages_per_book*6 if print_kind!=1 else bingo_pages_per_book*6*2
                    print_id = addToPrintInfo(mydb, mycursor, perm, print_perm_count+perm-1, print_kind, session_id)
                    mycanvas = canvas.Canvas('books/{}-{}-({}-{}).pdf'.format(session_name, book_kind[print_kind], perm, print_perm_count+perm-1))
                    mycanvas.setPageSize(landscape(A4))
                    mycanvas.setFont('Helvetica', 16)
                    new_canvas = False
                    card_idx = 0
                    curr_color_idx = 0
                    page_number = 0
                page_number += 1
                rgb = colors[panel_colors[curr_color_idx]]
                mycanvas.setFillColorRGB(1, 1, 1)
                mycanvas.rect((card_idx%3)*(page_width/3)*mm, 0, page_width*mm/3, page_height*mm, fill=1)
                mycanvas.setFillColorRGB(1, 1, 1)
                mycanvas.rect(((card_idx%3)*(page_width/3)+left_margin)*mm, get_real_y((top_margin+logo_height)*mm), logo_width*mm, logo_height*mm, stroke=0, fill=1)
                mycanvas.setFont("Helvetica", 16)
                mycanvas.drawImage(logo_path, ((card_idx%3)*(page_width/3)+left_margin)*mm, get_real_y((top_margin+logo_height)*mm), logo_width*mm, logo_height*mm)
                mycanvas.rect(((card_idx%3)*(page_width/3)+left_margin)*mm, get_real_y((top_margin+logo_height+panel_interval+page_title_height)*mm), logo_width*mm, page_title_height*mm, stroke=0, fill=1)
                mycanvas.setFillColorRGB(0, 0, 0)
                x = ((card_idx%3)*(page_width/3)+left_margin)*mm + logo_width*mm/2
                mycanvas.drawCentredString(x, get_real_y((top_margin+logo_height+page_title_height+page_title_interval-1)*mm), page_title)

                cell_width = (page_width/3-left_margin-panel_number_height-panel_interval-right_margin)/9
                cell_height = (page_height-top_margin*3-logo_height-page_title_height-panel_interval*7)/18

                query = "select panel_1.panel_id, panel_1.numbers, panel_2.panel_id, panel_2.numbers, panel_3.panel_id, panel_3.numbers, panel_4.panel_id, panel_4.numbers, panel_5.panel_id, panel_5.numbers,\
                    panel_6.panel_id, panel_6.numbers, panel_1.card_id from panel_1, panel_2, panel_3, panel_4, panel_5, panel_6 where panel_{:d}.panel_id='{:d}' and panel_{:d}.card_id=panel_1.card_id and panel_{:d}.card_id=panel_2.card_id \
                    and panel_{:d}.card_id=panel_3.card_id and panel_{:d}.card_id=panel_4.card_id and panel_{:d}.card_id=panel_5.card_id and panel_{:d}.card_id=panel_6.card_id".format(nTableId, perm, nTableId,
                    nTableId, nTableId, nTableId, nTableId, nTableId)
                mycursor.execute(query)
                myresult = mycursor.fetchall()
                left = (card_idx%3)*(page_width/3)+left_margin+panel_number_height+panel_vertical_interval
                top = top_margin+logo_height+panel_interval*2+page_title_height
                if len(myresult) >= 1:
                    mycanvas.setStrokeColorRGB(0, 0, 0) 
                    for i in range(0, 6):
                        panel_id = myresult[0][i*2]
                        addTicketToDatabase(mydb, mycursor, myresult[0][12], panel_id, session_id, print_id, panel_colors[curr_color_idx])
                        panel_numbers_str = myresult[0][i*2+1]
                        if i > 0:
                            top += (cell_height+panel_interval)
                        mycanvas.setFillColorRGB(rgb[0]/255, rgb[1]/255, rgb[2]/255)
                        mycanvas.rect((left+cell_width*9/4)*mm, get_real_y(top*mm), cell_width*9*mm/2, panel_interval*mm, stroke=1, fill=1)
                        mycanvas.setFont("Helvetica", 14)
                        mycanvas.setFillColorRGB(1, 1, 1)
                        mycanvas.drawCentredString((left+cell_width*9/2)*mm, get_real_y((top-1)*mm), panel_colors[curr_color_idx])#panel_interval-
                        for j in range(0, 3):
                            if j > 0:
                                top += cell_height
                            for k in range(0, 9):
                                ch = str(int(panel_numbers_str[j*2*9+k*2:j*2*9+(k+1)*2]))
                                if ch=='0':
                                    ch = ''
                                mycanvas.setFillColorRGB(1, 1, 1)
                                mycanvas.rect((left+k*cell_width)*mm, get_real_y((top+cell_height)*mm), cell_width*mm, cell_height*mm, stroke=1, fill=1)
                                mycanvas.setFillColorRGB(0, 0, 0)
                                x = (left+k*cell_width)*mm+cell_width*mm/2
                                mycanvas.setFont("Helvetica", 16)
                                mycanvas.drawCentredString(x, get_real_y((top+cell_height-1)*mm), ch) 
                        mycanvas.saveState()
                        mycanvas.rotate(90)
                        x = (card_idx%3)*(page_width/3)+left_margin+panel_number_height
                        y = top+cell_height
                        mycanvas.setFillColorRGB(1, 1, 1)
                        mycanvas.rect(get_real_y(y*mm), -x*mm, cell_height*3*mm, panel_number_height*mm, stroke=0, fill=1)
                        mycanvas.setFont("Helvetica", 10)
                        panel_id_str = "{:07d}".format(int(panel_id))
                        mycanvas.setFillColorRGB(0, 0, 0)
                        mycanvas.drawCentredString(get_real_y((y-cell_height*3/2)*mm), (-x+1)*mm, panel_id_str)
                        mycanvas.restoreState()
                    mycanvas.setFont("Helvetica", 12)
                    mycanvas.setFillColorRGB(1, 1, 1)
                    text_width = stringWidth(date_string, 'Helvetica', 12)
                    mycanvas.rect((card_idx%3)*(page_width/3)*mm+(page_width*mm/3-text_width)/2, get_real_y((top+cell_height+panel_interval+page_title_height)*mm), text_width, page_title_height*mm, stroke=0, fill=1)
                    mycanvas.setFillColorRGB(0, 0, 0)
                    x = (card_idx%3)*(page_width/3)*mm+(page_width*mm/3-text_width)/2+text_width/2
                    mycanvas.drawCentredString(x, get_real_y((top+cell_height+page_title_interval+page_title_height-1)*mm), date_string) 
                    x = (card_idx%3)*(page_width/3)*mm
                    mycanvas.setFont("Helvetica", 16)
                    mycanvas.drawString(x, 5, panel_colors[curr_color_idx])
                    x = (card_idx%3)*(page_width/3)*mm+page_width*mm/6
                    mycanvas.drawCentredString(x, 5, str(page_number))
                    curr_color_idx += 1
                    if curr_color_idx==panel_color_cnt:
                        curr_color_idx = 0
                card_idx += 1
                if print_double==True and bingo_pages_per_book==card_idx:
                    curr_color_idx = 0
                if (print_double==False and bingo_pages_per_book==card_idx) or (print_double==True and bingo_pages_per_book*2==card_idx):
                    mycanvas.showPage()
                    mycanvas.save()
                    book_id += 1
                    new_canvas = True
                elif card_idx%3 == 0 or perm==end_panel:
                    mycanvas.showPage()
            if new_canvas==False:
                mycanvas.save()
        show_message('Success', 'Successfully generated')
    except Exception as e:
        show_message('Error', 'You met some exception.')
    return


def makePdfUsingCustomerNameWithPyPDF2(filename, session_id, kind, perm_range_from, perm_range_to, name):
    try:
        print_kind = 0
        if kind=='double':
            print_kind = 1
        elif kind=='sheet':
            print_kind = 2
        packet = io.BytesIO()
        mycanvas = canvas.Canvas(packet)
        mycanvas.setPageSize(landscape(A4))
        mycanvas.setFont('Helvetica', 16)
        curr_color_idx = 0
        ret = 1
        if ret==1:
            card_idx = 0
            new_canvas = False
            book_id = 1
            # for perm in range(perm_range_from, perm_range_to+1):
            #     nTableId = perm % 6
            #     if nTableId == 0:
            #         nTableId = 6
            #     if nTableId != 1:
            #         continue
                
            #     mycanvas = canvas.Canvas(packet)
            #     mycanvas.setPageSize(landscape(A4))
            #     mycanvas.setFont('Helvetica', 16)
            #     new_canvas = False
            #     card_idx = 0
            #     page_number = 0
            #     page_number += 1
            #     mycanvas.setFont("Helvetica", 16)
            #     x = (card_idx%3)*(page_width/3)*mm+page_width*mm/4
            #     mycanvas.drawCentredString(x, 5, name)
            #     # mycanvas.showPage()
            #     mycanvas.save()
            #     book_id += 1
            mycanvas = canvas.Canvas(packet)
            mycanvas.setPageSize(landscape(A4))
            mycanvas.setFont('Helvetica', 16)
            new_canvas = False
            card_idx = 0
            page_number = 0
            page_number += 1
            mycanvas.setFont("Helvetica", 16)
            x = (card_idx%3)*(page_width/3)*mm+page_width*mm/4
            mycanvas.drawCentredString(x, 5, name)
            # mycanvas.showPage()
            mycanvas.save()
            book_id += 1
            
            packet.seek(0)
            new_pdf = PdfFileReader(packet)
            existing_pdf = PdfFileReader(open(filename, "rb"))
            output = PdfFileWriter()
            page = existing_pdf.getPage(0)
            page.mergePage(new_pdf.getPage(0))
            output.addPage(page)
            outputStream = open(filename, "wb")
            output.write(outputStream)
            outputStream.close()
            
        show_message('Success', 'Successfully generated')
    except Exception as e:
        show_message('Error', 'You met some exception.')
    return


def makePdfUsingCustomerNameWithPdfrw(filename, session_id, kind, sold_from, sold_to, name, perm_range_from, perm_range_to):
    try:
        print_kind = 0
        if kind=='double':
            print_kind = 1
        elif kind=='sheet':
            print_kind = 2
        packet = io.BytesIO()
        mycanvas = canvas.Canvas(packet)
        mycanvas.setPageSize(landscape(A4))
        mycanvas.setFont('Helvetica', 16)
        curr_color_idx = 0
        ret = 1
        if ret==1:
            card_idx = 0
            for perm in range(perm_range_from, perm_range_to+1):
                nTableId = perm % 6
                if nTableId != 1:
                    continue
                x = (card_idx%3)*(page_width/3)*mm+page_width*mm/4
                if perm >= sold_from and perm <= sold_to:
                    mycanvas.drawCentredString(x, 5, name)
                card_idx += 1
                if card_idx==3:
                    mycanvas.showPage()
                    card_idx = 0
            mycanvas.showPage()
            mycanvas.save()

            packet.seek(0)
            new_pdf = PdfReader(packet)
            existing_pdf = PdfReader(filename)
            output = PdfWriter()
            for i in range(len(existing_pdf.pages)):
                merger = PageMerge(existing_pdf.pages[i])
                merger.add(new_pdf.getPage(i)).render()
            output.write(filename, existing_pdf)
            
        show_message('Success', 'Successfully generated')
    except Exception as e:
        show_message('Error', 'You met some exception.')
    return
