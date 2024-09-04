SELECT 
  list_aggregate(tau_pt, 'sum'),
  list_aggregate(tau_eta, 'sum'),
  list_aggregate(jet_pt, 'sum'),
  list_aggregate(jet_eta, 'sum'),
  list_aggregate(muon_pt, 'sum'),
  list_aggregate(muon_eta, 'sum'),
  list_aggregate(el_pt, 'sum'),
  list_aggregate(el_eta, 'sum'),
  list_aggregate(ph_pt, 'sum'),
  list_aggregate(ph_eta, 'sum')  
FROM singleMu;