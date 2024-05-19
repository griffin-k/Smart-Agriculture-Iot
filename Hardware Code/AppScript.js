function doGet(e) {
    var sheetId = "1J7yYX4wexrCmyqr8SW2LzbwXTWboDhYqNSpk-Mut4TA"; 
    var sheet = SpreadsheetApp.openById(sheetId).getActiveSheet();
  
    // Fetch values from the Google Sheet
    var temperature = sheet.getRange("A1").getValue();
    var humidity = sheet.getRange("B1").getValue();
    var soilMoisture = sheet.getRange("C1").getValue();
    var otherValue = sheet.getRange("D1").getValue();
  
    // Check if new values are provided in the URL parameters
    if (e.parameter.temperature !== undefined && e.parameter.humidity !== undefined && e.parameter.soilMoisture !== undefined ) {
      // Update values in the Google Sheet with the new values provided
      temperature = parseFloat(e.parameter.temperature);
      humidity = parseFloat(e.parameter.humidity);
      soilMoisture = parseFloat(e.parameter.soilMoisture);
      otherValue = parseFloat(e.parameter.otherValue);
  
      try {
        sheet.getRange("A1").setValue(temperature);
        sheet.getRange("B1").setValue(humidity);
        sheet.getRange("C1").setValue(soilMoisture);
        sheet.getRange("D1").setValue(otherValue);
        return ContentService.createTextOutput("Data updated successfully");
      } catch (error) {
        Logger.log("Error updating sheet: " + error);
        return ContentService.createTextOutput("Error updating sheet").setStatusCode(500);
      }
    }
  
    // Create a JSON object with the fetched values
    var jsonObject = {
      "temperature": temperature,
      "humidity": humidity,
      "soilMoisture": soilMoisture,
      "otherValue": otherValue
    };
  
    // Return the JSON object as the response
    return ContentService.createTextOutput(JSON.stringify(jsonObject)).setMimeType(ContentService.MimeType.JSON);
  }
  