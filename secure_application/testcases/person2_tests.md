# Person 2 Test Cases

## TC01 - Schedule Appointment

Input:
Patient ID = 1
Doctor = Dr. Sharma
Date = 12-08-2026
Time = 10:30

Expected Result:
Appointment should be created.

Actual Result:
Appointment created successfully.

Status:
PASS


## TC02 - Appointment Lookup

Input:
Patient ID = 1

Expected Result:
Patient 1 appointment should be displayed.

Actual Result:
Appointment displayed.

Status:
PASS


## TC03 - Add Prescription

Input:
Patient ID = 1
Medicine = Paracetamol
Dosage = 500mg

Expected Result:
Prescription should be stored.

Actual Result:
Prescription stored.

Status:
PASS


## TC04 - Billing

Input:
Patient ID = 1
Amount = 1500

Expected Result:
Bill should be generated.

Actual Result:
Bill generated.

Status:
PASS


## TC05 - SQL Injection

Input:
1' OR '1'='1

Expected Result:
Application should demonstrate SQL injection in the vulnerable version.

Actual Result:
Multiple appointment records may be returned.

Status:
VULNERABLE


## TC06 - Broken Access Control

Scenario:
User associated with Patient 1 requests billing information
for Patient 2.

Expected Result:
Access should be denied.

Actual Result:
Patient 2 billing information is displayed because
authorization is not checked.

Status:
VULNERABLE
