SELECT 
  list_aggregate(tau_pt, 'median'),
  list_aggregate(tau_eta, 'median'),
  list_aggregate(jet_pt, 'median'),
  list_aggregate(jet_eta, 'median'),
  list_aggregate(muon_pt, 'median'),
  list_aggregate(muon_eta, 'median'),
  list_aggregate(el_pt, 'median'),
  list_aggregate(el_eta, 'median'),
  list_aggregate(ph_pt, 'median'),
  list_aggregate(ph_eta, 'median')  
FROM singleMu;