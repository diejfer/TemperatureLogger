function doGet(postData) {
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var rawdata = ss.getSheetByName("rawdata");
  var options = ss.getSheetByName("options");
  
  rawdata.insertRowBefore(1);
  
  var now = new Date();
  var data = [
    [now, postData.parameter.temp, postData.parameter.hum, postData.parameter.sensor]
  ];
  
  rawdata.getRange(1, 1, 1, 4).setValues(data);
  
  var response = options.getRange("B1").getValue();
  
  var content = ContentService.createTextOutput(response);
  
  return content;
}
