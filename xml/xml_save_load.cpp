
bool PouLadWidget::savePOU(QDomDocument &doc, QTextStream &stream)
{
    QDomElement root = doc.createElement("gsklad");     //create root
    //address
    QDomElement Address = doc.createElement("iniAddress");
    Address.setAttribute("address",QString("%1").arg(initAddress));
    root.appendChild(Address);

    QDomElement pouType = doc.createElement("pouType");
    pouType.setAttribute("type",QString("%1").arg(EditorTypeStrMap[getPOUType()]));
    root.appendChild(pouType);

    QDomElement projName = doc.createElement("projName");
    projName.setAttribute("name",QString("%1").arg(getFileName()));
    root.appendChild(projName);

    //prog segment
    for (int i = 0; i < netBaseList.count(); ++i)
    {
        QDomElement element = doc.createElement("net");
        element.setAttribute("id",QString("%1").arg(netBaseList.at(i)->getLabelCount()));

        QDomElement netsize = doc.createElement("netsize");
        netsize.setAttribute("width", QString("%1").arg(netBaseList.at(i)->width()));
        netsize.setAttribute("height", QString("%1").arg(netBaseList.at(i)->height()));
        element.appendChild(netsize);

        QDomElement titleannotation = doc.createElement("titleannotation");
        titleannotation.setAttribute("title", QString("%1").arg(netBaseList.at(i)->getTitel()));
        titleannotation.setAttribute("annotation", QString("%1").
                                     arg(netBaseList.at(i)->getAnnotationText()));
        element.appendChild(titleannotation);

        QDomElement attribute = doc.createElement("attribute");
        attribute.setAttribute("width", QString("%1").arg(netBaseList.at(i)->drawLadWidget->width()));
        attribute.setAttribute("height", QString("%1").arg(netBaseList.at(i)->drawLadWidget->height()));

        QDomElement netelement = doc.createElement("elements");
        netelement.appendChild(attribute);
        element.appendChild(netelement);

        netBaseList.at(i)->drawLadWidget->sortElemList();
        netBaseList.at(i)->drawLadWidget->toCplFormat();
        //ladWidgetList.at(i)->drawLadWidget->save(doc, netelement);
        netBaseList.at(i)->drawLadWidget->saveElement(doc, netelement);
        root.appendChild(element);
    }
    //编辑模式要与文件保存的模式一样，否则会有读写错误
    QString strHead("version =\"1.0\" encoding =\"UTF-8\"" );
    doc.appendChild(doc.createProcessingInstruction("xml" ,strHead ));
    stream << doc.toString();
    root.save(stream,2);
    return true;
}


bool PouLadWidget::loadPOU(QFile *file)
{
    netBaseList.clear();
    QXmlStreamReader reader(file);
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name() == "gsklad")
            {
                reader.readNext();
            }

            // address
            if( "iniAddress" == reader.name())
            {
                QXmlStreamAttributes attribute = reader.attributes();
                if(attribute.hasAttribute("address"))
                    initAddress = attribute.value("address").toString().toInt();
            }

            // type
            if( "pouType" == reader.name())
            {
                QXmlStreamAttributes attribute = reader.attributes();
                if(attribute.hasAttribute("type")) {
                    QString ptype;
                    ptype = attribute.value("type").toString();

                    if(ptype.compare("LAD")) setPOUType( LAD_EDITOR);
                    if(ptype.compare("STL")) setPOUType( STL_EDITOR);

                }
            }

            //document 
            if( "projName" == reader.name())
            {
                QXmlStreamAttributes attribute = reader.attributes();
                if(attribute.hasAttribute("name")) {
                    QString name = attribute.value("name").toString();
                    setFileName(name);
                }
            }

            //parse if net element
            if(reader.name() == "net")
            {
                addNet();
            }
            
            //title notion
            if(reader.name() == "titleannotation")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("title"))
                {
                    curNet()->setTitel(attributes.value("title").toString());
                }
                if(attributes.hasAttribute("annotation"))
                {
                    curNet()->setAnnotationText(attributes.value("annotation").toString());
                }
            }
            if(reader.name() == "elements")
            {
                curNet()->parse(reader);
            }
        }
        reader.readNext();
    }
    // 删除所有设备、数据，并将其重置为初始状态
    reader.clear();
    sortNetWidgetSlot();
    return true;

}
