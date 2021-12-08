# -*- coding: UTF-8 -*-
'''=================================================
@Project -> File   ：SR-SToF100-test-system -> PDF
@IDE    ：PyCharm
@Author ：BJW
@Date   ：2021/5/31 11:41
=================================================='''
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, PageBreak, Table, TableStyle
from reportlab.platypus import Image as reportImage
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER, TA_LEFT
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont

pdfmetrics.registerFont(TTFont('pingbold', './ttf/msyh.ttf'))


# 生成PDF文件
class PDFGenerator:
    """PDF类

    生成PDF
    """

    def __init__(self):
        """构造函数

        规定PDF保存路径，PDF的格式规范
        """
        self.file_path = './PDFs/'  # 保存路径，PDFs文件夹
        self.title_style = ParagraphStyle(name="TitleStyle", fontName="pingbold", fontSize=48, alignment=TA_LEFT, )
        self.sub_title_style = ParagraphStyle(name="SubTitleStyle", fontName="pingbold", fontSize=32,
                                              textColor=colors.HexColor(0x666666), alignment=TA_LEFT, )
        self.content_style = ParagraphStyle(name="ContentStyle", fontName="pingbold", fontSize=18, leading=25,
                                            spaceAfter=20,
                                            underlineWidth=1, alignment=TA_LEFT, )
        self.foot_style = ParagraphStyle(name="FootStyle", fontName="pingbold", fontSize=14,
                                         textColor=colors.HexColor(0xB4B4B4),
                                         leading=25, spaceAfter=20, alignment=TA_CENTER, )
        self.table_title_style = ParagraphStyle(name="TableTitleStyle", fontName="pingbold", fontSize=20, leading=25,
                                                spaceAfter=10, alignment=TA_LEFT, )
        self.sub_table_style = ParagraphStyle(name="SubTableTitleStyle", fontName="pingbold", fontSize=16, leading=25,
                                              spaceAfter=10, alignment=TA_LEFT, )
        self.basic_style = TableStyle([('FONTNAME', (0, 0), (-1, -1), 'pingbold'),
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
        self.common_style = TableStyle([('FONTNAME', (0, 0), (-1, -1), 'pingbold'),
                                        ('FONTSIZE', (0, 0), (-1, -1), 12),
                                        ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                                        ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                                        ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
                                        ('GRID', (0, 0), (-1, -1), 0.5, colors.black),
                                        ])

    def genTaskPDF(self):
        """生成PDF

        pcb_data的信息通过表格显示，UART等外设的通信测试结果折线图表示
        """
        # self.pcb_data = pcb_data
        # print("pcb_numb编号：" + self.pcb_data[7][1])  # 用pcb_nub作为二维码数据
        # mainwindow.ChildDialog.textBrowser.append("pcb_numb编号：" + self.pcb_data[7][1])
        filename = 'PDF_result'
        self.filename = filename
        story = []
        # 首页内容
        story.append(Spacer(1, 20 * mm))
        imgofsr = reportImage('./Icon.sr.jpg')
        imgofsr.drawHeight = 20 * mm
        imgofsr.drawWidth = 100 * mm
        imgofsr.hAlign = TA_LEFT
        story.append(imgofsr)
        story.append(Spacer(1, 10 * mm))
        story.append(Paragraph("测试报告", self.title_style))
        story.append(Spacer(1, 20 * mm))
        story.append(Paragraph("Test Report of SToF100", self.sub_title_style))
        story.append(Spacer(1, 45 * mm))
        story.append(Paragraph("报告编号：" + self.pcb_data[0]['report_code'], self.content_style))
        story.append(Paragraph("测试名称：" + self.pcb_data[0]['task_name'], self.content_style))
        story.append(Paragraph("报告日期：" + self.pcb_data[0]['report_date'], self.content_style))
        story.append(Paragraph(" 测试人：" + self.pcb_data[0]['report_creator'], self.content_style))
        story.append(Spacer(1, 55 * mm))
        story.append(Paragraph("内部文档，请勿外传", self.foot_style))
        story.append(PageBreak())

        # 表格允许单元格内容自动换行格式设置
        stylesheet = getSampleStyleSheet()
        body_style = stylesheet["BodyText"]
        body_style.wordWrap = 'CJK'
        body_style.fontName = 'pingbold'
        body_style.fontSize = 12

        # # PCB测试数据
        # story.append(Paragraph("PCB测试数据", self.table_title_style))
        # story.append(Spacer(1, 3 * mm))
        # task_table = Table(self.pcb_data[1:], colWidths=[60 * mm, 120 * mm], rowHeights=12 * mm,
        #                    style=self.common_style)  # 调整宽度高度
        # story.append(task_table)

        # PCB通信测试结果，插入图片
        # UART
        story.append(Spacer(1, 20 * mm))
        img_DistanceImage_path = "/Test/CAN-DATA_0526.png"
        img_Distance = reportImage(img_DistanceImage_path)
        img_Distance.drawHeight = 80 * mm
        img_Distance.drawWidth = 130 * mm
        img_Distance.hAlign = TA_LEFT
        story.append(img_Distance)

        # CAN
        story.append(Spacer(1, 20 * mm))
        img_StrengthImage_path = "c:\SR-SToF100-test-system\Strenght_image.png"
        img_Strength = reportImage(img_StrengthImage_path)
        img_Strength.drawHeight = 80 * mm
        img_Strength.drawWidth = 130 * mm
        img_Strength.hAlign = TA_LEFT
        story.append(img_Strength)

        doc = SimpleDocTemplate(self.file_path + self.filename + ".pdf",
                                leftMargin=20 * mm, rightMargin=20 * mm, topMargin=20 * mm, bottomMargin=20 * mm)
        doc.build(story)
        # print('已经生成PDF，文件名为 %s.pdf，请查看！' % self.pcb_data[7][1])  # 用pcb_num命名
        # mainwindow.ChildDialog.textBrowser.append('已经生成PDF，文件名为 %s.pdf，请查看！' % self.pcb_data[7][1])

# if __name__ == "__main__":
