# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> PDF_1
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/6/1 10:55
=================================================='''
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Image, PageBreak, Table, TableStyle
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.platypus import Image as reportImage
from reportlab.lib.units import mm
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_LEFT
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
import time

pdfmetrics.registerFont(TTFont('pingbold', 'PingBold.ttf'))
pdfmetrics.registerFont(TTFont('ping', 'ping.ttf'))
pdfmetrics.registerFont(TTFont('hv', 'Helvetica.ttf'))

# stof100 = 1

# 生成PDF文件
class PDFGenerator:
    def __init__(self, filename):
        self.filename = filename
        self.file_path = ('..\\PDFs\\PDF')
        self.title_style = ParagraphStyle(name="TitleStyle", fontName="pingbold", fontSize=48, alignment=TA_LEFT,)
        self.sub_title_style = ParagraphStyle(name="SubTitleStyle", fontName="hv", fontSize=32,
                                              textColor=colors.HexColor(0x666666), alignment=TA_LEFT, )
        self.content_style = ParagraphStyle(name="ContentStyle", fontName="ping", fontSize=18, leading=25, spaceAfter=20,
                                            underlineWidth=1, alignment=TA_LEFT, )
        self.foot_style = ParagraphStyle(name="FootStyle", fontName="ping", fontSize=14, textColor=colors.HexColor(0xB4B4B4),
                                         leading=25, spaceAfter=20, alignment=TA_CENTER, )
        self.table_title_style = ParagraphStyle(name="TableTitleStyle", fontName="pingbold", fontSize=20, leading=25,
                                                spaceAfter=10, alignment=TA_LEFT, )
        self.sub_table_style = ParagraphStyle(name="SubTableTitleStyle", fontName="ping", fontSize=16, leading=25,
                                                spaceAfter=10, alignment=TA_LEFT, )
        self.basic_style = TableStyle([('FONTNAME', (0, 0), (-1, -1), 'ping'),
                                       ('FONTSIZE', (0, 0), (-1, -1), 12),
                                       ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                                       ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                                       ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
                                       # 'SPAN' (列,行)坐标
                                       ('SPAN', (1, 0), (3, 0)),
                                       ('SPAN', (1, 1), (3, 1)),
                                       ('SPAN', (1, 2), (3, 2)),
                                       ('SPAN', (1, 5), (3, 5)),
                                       ('SPAN', (1, 6), (3, 6)),
                                       ('SPAN', (1, 7), (3, 7)),
                                       ('GRID', (0, 0), (-1, -1), 0.5, colors.black),
                                       ])
        self.common_style = TableStyle([('FONTNAME', (0, 0), (-1, -1), 'ping'),
                                      ('FONTSIZE', (0, 0), (-1, -1), 12),
                                      ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                                      ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                                      ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
                                      ('GRID', (0, 0), (-1, -1), 0.5, colors.black),
                                     ])

    def genTaskPDF(self, home_data, task_data, basic_data):
        story = []

        # 首页内容
        story.append(Spacer(1, 5 * mm))
        img = Image('../Icon/sr.jpg')
        img.drawHeight = 40 * mm
        img.drawWidth = 150 * mm
        img.hAlign = TA_LEFT
        story.append(img)
        story.append(Spacer(1, 20 * mm))
        story.append(Paragraph("测试报告", self.title_style))
        story.append(Spacer(1, 20 * mm))
        story.append(Paragraph("Test Report of SToF100", self.sub_title_style))
        story.append(Spacer(1, 50 * mm))
        story.append(Paragraph("报告编号：" + home_data['report_code'], self.content_style))
        story.append(Paragraph("计划名称：" + home_data['task_name'], self.content_style))
        story.append(Paragraph("报告日期：" + home_data['report_date'], self.content_style))
        story.append(Paragraph(" 负责人：" + home_data['report_creator'], self.content_style))
        story.append(Spacer(1, 30 * mm))
        story.append(Paragraph("内部文档，请勿外传", self.foot_style))
        story.append(PageBreak())

        # 表格允许单元格内容自动换行格式设置
        stylesheet = getSampleStyleSheet()
        body_style = stylesheet["BodyText"]
        body_style.wordWrap = 'CJK'
        body_style.fontName = 'ping'
        body_style.fontSize = 12

        # 测试计划
        story.append(Paragraph("SToF100测试结果", self.table_title_style))
        story.append(Spacer(1, 3 * mm))
        task_table = Table(task_data, colWidths=[25 * mm, 141 * mm], rowHeights=12 * mm, style=self.common_style)
        story.append(task_table)

        story.append(Spacer(1, 10 * mm))

        # 基础参数
        story.append(Paragraph("基础参数", self.sub_table_style))
        basic_table = Table(basic_data, colWidths=[25*mm, 61*mm, 25*mm, 55*mm], rowHeights=12 * mm, style=self.basic_style)
        story.append(basic_table)

        story.append(Spacer(1, 10 * mm))



        #插入图片
        story.append(Paragraph("SToF100测试结果图", self.table_title_style))
        story.append(Spacer(1, 10 * mm))
        img_uart_path = '../Matplot_data/Distance_image.png'
        img_uart = reportImage(img_uart_path)
        img_uart.drawHeight = 140 * mm
        img_uart.drawWidth = 150 * mm
        img_uart.hAlign = TA_LEFT
        story.append(img_uart)

        doc = SimpleDocTemplate(self.file_path + self.filename + ".pdf",
                                leftMargin=20 * mm, rightMargin=20 * mm, topMargin=20 * mm, bottomMargin=20 * mm)
        doc.build(story)


class PDF_result():
    def pdf_result():
        # 格式化成2016-03-20 11:45:39形式
        data = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
        """主程序"""
        #获得的产品数据
        home_data = {'report_code': '1',
                   'task_name':'SToF100',
                   'report_date': data ,
                   'report_creator':'BJW'
                  }
        task_data = [
                        ['计划ID', 'stof100'],
                        ['计划ID','1'],
                        ['计划ID','1']
                    ]
        basic_data = [
                        ['测试ID','1'],
                        ['测试ID','2','测试','3']
                    ]

        #PDF类，读取产品数据并生成PDF
        test = PDFGenerator("_report")
        test.genTaskPDF(home_data,task_data,basic_data)

if __name__ == "__main__":
    PDF_result.pdf_result()
