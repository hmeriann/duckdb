SELECT 
  list_aggregate(tau_pt, 'max'),
  list_aggregate(tau_eta, 'max'),
  list_aggregate(jet_pt, 'max'),
  list_aggregate(jet_eta, 'max'),
  list_aggregate(muon_pt, 'max'),
  list_aggregate(muon_eta, 'max'),
  list_aggregate(el_pt, 'max'),
  list_aggregate(el_eta, 'max'),
  list_aggregate(ph_pt, 'max'),
  list_aggregate(ph_eta, 'max')  
FROM singleMu;