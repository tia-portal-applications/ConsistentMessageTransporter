<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
   <xsl:template match="/">
 
      <Tags>
         <xsl:for-each select="/Tags/tag">          
            <xsl:element name = "tag">            
            <xsl:choose>     
                 <xsl:when test="value='start'">
                        <xsl:choose>
                             <xsl:when test="name='tag1'">true</xsl:when>
                             <xsl:otherwise>start</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                   <xsl:when test="value='end'">
                        <xsl:choose>
                             <xsl:when test="name='tag1'">false</xsl:when>
                             <xsl:otherwise>end</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                    <xsl:when test="value='run'">
                        <xsl:choose>
                             <xsl:when test="name='wincc1'">true</xsl:when>
                             <xsl:otherwise>run</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                   <xsl:when test="value='stop'">
                        <xsl:choose>
                             <xsl:when test="name='wincc1'">false</xsl:when>
                             <xsl:otherwise>stop</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                    <xsl:when test="value='1'">
                    <xsl:choose>
                             <xsl:when test="name='wincc1'">true</xsl:when>
                             <xsl:when test="name='tag1'">true</xsl:when>
                             <xsl:otherwise>1</xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>
                    <xsl:when test="value='0'">
                    <xsl:choose>
                             <xsl:when test="name='wincc1'">false</xsl:when>
                             <xsl:when test="name='tag1'">false</xsl:when>
                             <xsl:otherwise>0</xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>
                   


                   <xsl:when test="value='true'">
                    <xsl:choose>
                             <xsl:when test="name='wincc1'">run</xsl:when>
                             <xsl:when test="name='tag1'">start</xsl:when>
                             <xsl:otherwise>true</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                   <xsl:when test="value='false'">
                    <xsl:choose>
                             <xsl:when test="name='wincc1'">stop</xsl:when>
                             <xsl:when test="name='tag1'">end</xsl:when>
                             <xsl:otherwise>false</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
     

                   <xsl:when test="value &lt;0">less than zero</xsl:when>                                      
                   <xsl:when test="value &gt;1and value&lt;100">normal</xsl:when> 
                   <xsl:when test="value='100'">100</xsl:when> 
                   <xsl:when test="value&gt;100and value&lt;1000">over 100!</xsl:when> 
                   <xsl:when test="value&gt;=1000and value&lt;10000">over 1000!</xsl:when>  
                   <xsl:when test="value='onehundred'">100</xsl:when>
                   <xsl:when test="value='twohundred'">200</xsl:when>                     
                   <xsl:when test="value='threehundred'">300</xsl:when>
                   <xsl:when test="value='onethousand'">1000</xsl:when>                 
                   <xsl:when test="value='TRUE'">true</xsl:when>
                   <xsl:when test="value='FALSE'">false</xsl:when>
                   <xsl:when test="value='The value is not satisfaction filter'">The value is not satisfaction filter</xsl:when>
                   <xsl:otherwise>no match value to convert</xsl:otherwise>
            </xsl:choose>            
            </xsl:element>   
         </xsl:for-each>             
      </Tags> 
   </xsl:template>
</xsl:stylesheet>
