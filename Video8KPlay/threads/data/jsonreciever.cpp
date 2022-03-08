#include "jsonreciever.h"

JsonReciever::JsonReciever(QObject *parent): DataReciever(parent)
{

}
void JsonReciever::DoRecive(QByteArray bytes)//only for no-nested jason
{
    QByteArray data = bytes;
    if(data.isEmpty()) return;
    m_arrangData.append(data);

    int Spos = 0;
    int Epos1 = 0;
    int Epos2 = 0;
    QByteArray segData;
    while(true)
    {
        Spos = m_arrangData.indexOf("{",Spos);
        if(Spos < 0)
        {
            m_arrangData.clear();
            break;
        }
        Epos1 = m_arrangData.indexOf("}",Spos+1);
        Epos2 = m_arrangData.indexOf("{",Spos+1);
        if((Epos1 >0&&Epos1 <Epos2)||(Epos1 >0&&Epos2 <0))
        {
            segData = m_arrangData.mid(Spos,Epos1 - Spos +1);
            Spos = Epos1 + 1;
            emit sendJson(segData);

        }
        else if( (Epos1 < 0 && Epos2 >0)||(Epos2 >0&&Epos2 <Epos1) )
        {
           Spos = Epos2;
        }
        else
        {
             segData = m_arrangData.mid(Spos,m_arrangData.size()- Spos);
             m_arrangData.clear();
             m_arrangData = segData;
             break;
        }
    }
}
