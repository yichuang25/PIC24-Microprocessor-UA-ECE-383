import  wx
import  wx.html
from    time import localtime, strftime
     
# a quick and dirty class to create a LOG window
# we will use the wx.html.HtmlWindow class to make
# our life easy and create a bit of eye candy
class LogHtmlWindow(wx.Frame):
  def __init__(self, parent, title):
    wx.Frame.__init__(self, parent, -1, title)
    self.html = wx.html.HtmlWindow(self)
    self.Bind(wx.EVT_CLOSE, self.OnClose)
    
    if "gtk2" in wx.PlatformInfo:
      self.html.SetStandardFonts()
    self.html.SetPage(
        "<h3>Log started on %s</h3><p>" % 
        strftime("%a, %d %b %Y %H:%M:%S </h1><p>", localtime())
    )

  # don't close the window, just hide it.  The parent will kill
  # us when they close (or they are supposed too)
  def OnClose(self, event):
      self.Hide()
      
  # quick method to place INCOMING text into log window
  def putInText(self, s):
    self.html.AppendToPage("<b>%s</b><br>" % s )
  
  # quick method to place OUTGOING text into log window
  def putOutText(self, s):
    self.html.AppendToPage("<font color=\"green\"><i>%s</i></font><br>" % s )
    
   # quick method to place STATUS text into log window
  def putStatusText(self, s):
    self.html.AppendToPage("<font color=\"red\">%s: <b><i>%s</i></b></font><br>" % 
        (strftime("%a, %d %b %Y %H:%M:%S </h1><p>", localtime()), s)
    )
