<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
   <xsl:template match="/">
 
      <Tags>
         <xsl:for-each select="/Tags/tag">          
            <xsl:element name = "tag">            
            <xsl:choose>     
                   <xsl:when test="value &lt;0">less than zero</xsl:when>                   
                   <xsl:when test="value='0'">
                        <xsl:choose>
                             <xsl:when test="name='Order_Tag_1'">stop</xsl:when>
                             <xsl:otherwise>end</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                   <xsl:when test="value='1'">
                        <xsl:choose>
                             <xsl:when test="name='Order_Tag_1'">running</xsl:when>
                             <xsl:otherwise>start</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when> 
                   <xsl:when test="value &gt;1and value&lt;100">normal</xsl:when> 
                   <xsl:when test="value=100">onehundred</xsl:when> 
                   <xsl:when test="value&gt;=100and value&lt;1000">more than hundred</xsl:when> 
                   <xsl:when test="value&gt;=1000and value&lt;10000">
                        <xsl:choose>
                             <xsl:when test="name='Order_Tag_4'">
                                 <xsl:value-of select="./value div 1000">m</xsl:value-of></xsl:when>
                             <xsl:when test="name='Order_Tag_5'">
                                 <xsl:value-of select="./value div 1000"></xsl:value-of></xsl:when>
                             <xsl:otherwise>more than thousand</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>  



                   <xsl:when test="value='onehundred'">100</xsl:when>
                   <xsl:when test="value='twohundred'">200</xsl:when>                     
                   <xsl:when test="value='threehundred'">300</xsl:when>
                   <xsl:when test="value='onethousand'">
                        <xsl:choose>
                             <xsl:when test="name='Order_Tag_4'">
                                 <xsl:value-of select="1* 1000"></xsl:value-of>m</xsl:when>
                             <xsl:when test="name='Order_Tag_5'">
                                 <xsl:value-of select="1* 100"></xsl:value-of>g</xsl:when>
                             <xsl:otherwise>more than thousand</xsl:otherwise>
                        </xsl:choose>
                   </xsl:when>
                   <xsl:when test="value='start'">1</xsl:when>
                   <xsl:when test="value='running'">1</xsl:when> 
                   <xsl:when test="value='stop'">0</xsl:when>
                   <xsl:when test="value='end'">0</xsl:when>  
                   <xsl:when test="value='true'">true</xsl:when>
                   <xsl:when test="value='false'">false</xsl:when>
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