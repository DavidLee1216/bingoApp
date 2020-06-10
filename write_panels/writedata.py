import csv
import mysql.connector

def writecsv(start, end, username, pw, filename):
    mydb = mysql.connector.connect(
        host="localhost",
        user=username,
        password=pw,
        database="bingo"
    )
    mycursor = mydb.cursor()
    with open(filename, 'w', newline='\n', encoding='utf-8') as csvfile:
        if csvfile is None:
            return -1
        writer = csv.writer(csvfile, delimiter=',',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
        
        if mycursor:
            for i in range(start, end+1):
                panel_id = []
                panel_id.append("PANEL "+str(i))
                writer.writerow(panel_id)
                table_id = i%6
                if table_id==0:
                    table_id = 6
                query = "select * from panel_{} where panel_id='{}'".format(table_id, i)
                mycursor.execute(query)
                myresult = mycursor.fetchall()
                for x in myresult:
                    for jj in range(0, 3):
                        row_info = []
                        for ii in range(0, 9):
                            ch = str(int(x[2][jj*2*9+ii*2:jj*2*9+(ii+1)*2]))
                            if ch=='0':
                                ch = ''
                            row_info.append(ch)
                        writer.writerow(row_info)
        else:
            return -2
    mycursor.close()
    mydb.close()
    return 0


def connectDB():
    mydb = mysql.connector.connect(
        host="localhost",
        user="root",
        password="Quoted1216",
        database="bingo"
    )
    mycursor = mydb.cursor()
    return mycursor
