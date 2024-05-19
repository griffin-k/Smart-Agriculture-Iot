function doGet(e) {
    var sheetId = ""; // Replace with your actual sheet ID
    var sheet = SpreadsheetApp.openById(sheetId).getActiveSheet();
  
    // Fetch values from the Google Sheet
    var temperature = sheet.getRange("A1").getValue();
    var humidity = sheet.getRange("B1").getValue();
    var soilMoisture = sheet.getRange("C1").getValue();
    var otherValue = sheet.getRange("D1").getValue();
  
   
    if (e.parameter.temperature && e.parameter.humidity && e.parameter.soilMoisture && e.parameter.otherValue) {
      temperature = e.parameter.temperature;
      humidity = e.parameter.humidity;
      soilMoisture = e.parameter.soilMoisture;
      otherValue = e.parameter.otherValue;
  
      sheet.getRange("A1").setValue(temperature);
      sheet.getRange("B1").setValue(humidity);
      sheet.getRange("C1").setValue(soilMoisture);
      sheet.getRange("D1").setValue(otherValue);
  
      return ContentService.createTextOutput("Data updated successfully");
    }
  
  
    var jsonObject = {
      "temperature": temperature,
      "humidity": humidity,
      "soilMoisture": soilMoisture,
      "otherValue": otherValue
    };
  
   
    return ContentService.createTextOutput(JSON.stringify(jsonObject)).setMimeType(ContentService.MimeType.JSON);
  }
  